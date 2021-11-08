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

#include "reactor.hpp"
#include "template.hpp"
#include "ui_mgr.hpp"

namespace ui {

void Reactor::Root(const std::string& root)
{
	root_ = root;
}

std::string Reactor::Root() const
{
	return root_;
}

Reactor::Reactor(UiMgr& m):
	manager_(m),
	pTpl_(0)
{
}

void Reactor::SetOption(const std::string& opt, const std::string& value)
{
	options_[opt] = value;
}

std::pair<std::string, bool> Reactor::GetOption(const std::string& opt) const
{
	std::pair<std::string, bool> result("0", false);
	const OptionsContainer::const_iterator i = options_.find(opt);

	if(i != options_.end())
	{
		result.first = i->second;
		result.second = true;
	}

	return result;
}

void Reactor::ProcessQuery(const Query& q, std::string& r)
{
	if(q.Has(Tags::sortby) && q.Has(Tags::sort)) 
		UpdateSortOption(StaticName::fromString(q.Get<std::string>(Tags::sort)), StaticName::fromString(q.Get<std::string>(Tags::sortby)));

	query_ = q;
	try
	{
		ProcessQuery();
		if(pTpl_)
		{
			for(OptionsContainer::const_iterator i = options_.begin(); i != options_.end(); ++i)
				if(!pTpl_->IsSet(i->first))
					pTpl_->Put(i->first, i->second);
		       	pTpl_->Format(r);
		}
	}
	catch(std::exception& e)
	{
		r = std::string("Exception during processing ui query: ") + e.what();
	}

	if(pTpl_) delete pTpl_;
	pTpl_ = 0;

	//Finilize(r);
	query_.Reset();
}

Template Reactor::GetTemplate(const std::string& name)
{
	const std::string fullName = root_ + "/" + name + ".tpl";
	Template t(fullName);
	return t;
}

void Reactor::SelectTemplate(const std::string& name)
{
	assert(!pTpl_);
	pTpl_ = new Template;
	const std::string fullName = root_ + "/" + name + ".tpl";
	pTpl_->Load(fullName);
}

/*
void Reactor::Finilize(std::string& out)
{
	if(!pTpl_) throw std::runtime_error("No template selected");
	try 
	{
		pTpl_->Format(out);
	}
	catch (std::exception& e)
	{
		out = e.what();
	}

	delete pTpl_;
	pTpl_ = 0;
}
*/

SortOption Reactor::GetSortOption(Tags::Type key)
{
	SortOptionContainer::const_iterator i = sortOptions_.find(key);
	if(i != sortOptions_.end()) return i->second;
	return SortOption();
}

void Reactor::InitSortOption(Tags::Type key, Tags::Type sortby, bool forward)
{
	sortOptions_[key].Update(sortby);
	sortOptions_[key].Forward(forward);

	if(sortOptions_.size() > 500) sortOptions_.erase(sortOptions_.begin());
}

void Reactor::UpdateSortOption(Tags::Type key, Tags::Type sortby)
{
	sortOptions_[key].Update(sortby);

	if(sortOptions_.size() > 500) sortOptions_.erase(sortOptions_.begin());
}

} //namespace ui

