/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2011  Andrey Stroganov <savthe@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SRGROUP_HPP
#define SRGROUP_HPP

#include <list>

namespace ui {

class RatedId
{
public:
	RatedId(): id_(0), count_(0) {}
	void Add(Id id)
	{
		if(!id_) id_ = id;
		++count_;
	}

	Id id() const { return id_; }
	int Count() const { return count_; }
private:
	Id id_;
	int count_;
};

struct RatedIdPairComparer 
{
	template <typename T>
	bool operator()(T r1, T r2) const
	{
		return r1->second.Count() > r2->second.Count();
	}
};

class SearchResultGroup
{
public:
	SearchResultGroup(): fileSize_(0), sources_(0), rate_(0) {}

	bool Add(const SearchResult& r)
	{
		if(!sha1_.empty() && !r.hit.sha1.empty() && sha1_ != r.hit.sha1) return false;
		if(!ttr_.empty() && !r.hit.ttr.empty() && ttr_ != r.hit.ttr) return false;
		if(!ed2k_.empty() && !r.hit.ed2k.empty() && ed2k_ != r.hit.ed2k) return false;
		if(!md5_.empty() && !r.hit.md5.empty() && md5_ != r.hit.md5) return false;

		if(sha1_.empty()) sha1_ = r.hit.sha1;
		if(ttr_.empty()) ttr_ = r.hit.ttr;
		if(ed2k_.empty()) ed2k_ = r.hit.ed2k;
		if(md5_.empty()) md5_ = r.hit.md5;

		if(fileSize_ == 0) fileSize_ = r.hit.size;
		rate_ += (1024 / 8) * r.host.rate;

		names_[r.hit.dn].Add(r.id);
		++sources_;

		return true;
	}

	char Alpha() const
	{
		if(names_.empty()) return 0;

		NamesConstIterators iters;
		PopulateIterators(iters);

		Names::const_iterator m = names_.begin();
		for(Names::const_iterator i = names_.begin(); i != names_.end(); ++i)
			if(i->second.Count() > m->second.Count()) m = i;
		if(m->first.empty()) return 0;

		return m->first[0];
	}

	template <typename T>
	void DumpNames(T out) const
	{
		NamesConstIterators iters;
		PopulateIterators(iters);

		std::sort(iters.begin(), iters.end(), RatedIdPairComparer());

		for(NamesConstIterators::iterator i = iters.begin(); i != iters.end(); ++i)
			*out++ = std::make_pair<std::string, Id>((*i)->first, (*i)->second.id());
	}

	uint64 FileSize() const { return fileSize_; }
	hash::SHA1 Sha1() const { return sha1_; }
	hash::TTR Ttr() const { return ttr_; }
	hash::ED2K Ed2k() const { return ed2k_; }
	hash::MD5 Md5() const { return md5_; }
	uint Sources() const { return sources_; }
	uint Rate() const { return rate_; }

private:
	typedef std::map<std::string, RatedId> Names;
	typedef std::vector<Names::const_iterator> NamesConstIterators;
	void PopulateIterators(NamesConstIterators& v) const
	{
		v.reserve(names_.size());
		for(Names::const_iterator i = names_.begin(); i != names_.end(); ++i) v.push_back(i);
	}

	uint64 fileSize_;
	hash::SHA1 sha1_;
	hash::TTR ttr_;
	hash::ED2K ed2k_;
	hash::MD5 md5_;
	unsigned int sources_;
	unsigned int rate_;
	Names names_;
};

template <typename T, typename P>
void CreateOrderedSRGroups(T begin, T end, P out)
{
	std::list<T> iters;
	for(T i = begin; i != end; ++i) iters.push_back(i);

	while(!iters.empty())
	{
		SearchResultGroup g;
		bool added;
		do
		{
			added = false;
			for(typename std::list<T>::iterator i = iters.begin(); i != iters.end();)
			{
				const SearchResult& r = **i;
				if(g.Add(r))
				{
					added = true;
					i = iters.erase(i);
				}
				else ++i;
			}
		}
		while(added);

		*out++ = g;
	}
}

struct SRGSortPredicate
{
	SRGSortPredicate(Tags::Type c): c_(c) {}

	bool operator()(const SearchResultGroup& g1, const SearchResultGroup& g2) const
	{
		switch(c_)
		{
			case Tags::sources: return g1.Sources() < g2.Sources(); break;
			case Tags::size: return g1.FileSize() < g2.FileSize(); break;
			default: return g1.Alpha() < g2.Alpha();
		}
	}

	Tags::Type c_;
};

struct SearchResultWriter: public Writer
{
	SearchResultWriter(Template* p, const Template& grp, Id sid): Writer(p), tplGroup_(grp), sid_(sid) 
	{
	}

	void operator()(const SearchResultGroup& g)
	{
		typedef std::pair<std::string, Id> NamePair;

 		tplGroup_.ClearValues();
		size_t showNames = 2;

		std::vector<NamePair> names;
		g.DumpNames(std::back_inserter(names));

		std::string fileType;
		for(std::vector<NamePair>::iterator i = names.begin(); i != names.end() && showNames--; ++i)
		{
 			tplGroup_.Put("name", AdaptString(i->first));
 			tplGroup_.Put("rid", i->second);
 			tplGroup_.Put("sid", sid_);
 			if(fileType.empty())
				fileType = FileType(i->first);
 
 		}
 
 		tplGroup_.Put("size", HumanReadable(g.FileSize()));
 		tplGroup_.Put("type", fileType);
		tplGroup_.Put("sources", g.Sources());

 		Put("name", tplGroup_);
 		Put("size", HumanReadable(g.FileSize()));
 		Put("actual_size", RankedNumber(g.FileSize(), " "));
 		Put("type", fileType);
		Put("sources", g.Sources());
		Put("rate", HumanReadable(g.Rate()));
		Put("sha1", g.Sha1().toString());
		Put("ttr", g.Ttr().toString());
		Put("ed2k", g.Ed2k().toString());
		Put("md5", g.Md5().toString());
	}

	Template tplGroup_;
	Id sid_;
};

} //namespace ui

#endif //SRGROUP_HPP

