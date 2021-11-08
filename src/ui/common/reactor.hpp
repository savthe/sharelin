/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2010  Andrey Stroganov <savthe@gmail.com>

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

#ifndef UI_REACTOR_HPP
#define UI_REACTOR_HPP

#include <string>
#include <stdexcept>
#include <limits>

#include "ui/common/query.hpp"
#include "ui/common/tag.hpp"
#include "ui/common/sortoption.hpp"
#include "ui/common/template.hpp"
#include "types.hpp"

class UiMgr;

namespace ui {

class Reactor
{
public:
	Reactor(UiMgr&);
	virtual ~Reactor() {}
	void Root(const std::string& root);
	std::string Root() const;
	void ProcessQuery(const Query&, std::string&);

	virtual void UpdateSortOption(Tags::Type, Tags::Type);
	virtual SortOption GetSortOption(Tags::Type);
	virtual void InitSortOption(Tags::Type, Tags::Type, bool);
	virtual std::string SpaceString() const { return std::string(); }


	/*
	void StylePath(const std::string& path)
	{
		stylePath_ = path;
	}

	std::string StylePath() const 
	{
		return stylePath_;
	}
	*/

protected:
	template <typename Key, typename Value>
	void Put(const Key& key, const Value& value)
	{
		if(!pTpl_) throw std::runtime_error("No template selected");
		pTpl_->Put(key, value);
	}

	template <typename T>
	void SetOption(const std::string& opt, const T& value)
	{
		std::stringstream s;
		s << value;
		options_[opt] = s.str();
	}

	void SetOption(const std::string&, const std::string&);
	std::pair<std::string, bool> GetOption(const std::string&) const;
	virtual void ProcessQuery() = 0;
	virtual uint PageSize() const { return std::numeric_limits<uint>::max(); }
	const Query& CurrentQuery() const { return query_; }
	void CurrentQuery(const Query& q) { query_ = q; }
	Template* CurrentTemplate() const { return pTpl_; }
	void SelectTemplate(const std::string&);
	Template GetTemplate(const std::string&);

	typedef std::map <Tags::Type, SortOption> SortOptionContainer;
	typedef std::map<std::string, std::string> OptionsContainer;

	std::string root_;
	SortOptionContainer sortOptions_;
	OptionsContainer options_;
	UiMgr& manager_;

private:
//	void Finilize(std::string&);
	Template* pTpl_;
	Query query_;
};

} //namespace ui

#endif //UI_REACTOR_HPP

