/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2009-2011  Andrey Stroganov <savthe@gmail.com>

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

#ifndef G2_NODESMANAGER_HPP
#define G2_NODESMANAGER_HPP

#include "connections.hpp"

namespace g2 {

template <typename Key, typename Ptr>
class NodesManager
{
	typedef Connections<Key, Ptr> Nodes;
public:
	typedef typename Nodes::iterator iterator;
	typedef typename Nodes::const_iterator const_iterator;

	virtual ~NodesManager() {}
	iterator begin() { return nodes_.begin(); }
	iterator end() { return nodes_.end(); }
	const_iterator begin() const { return nodes_.begin(); }
	const_iterator end() const { return nodes_.end(); }

	Ptr get(const Key& key) const { return nodes_.get(key); }
	std::size_t count() const { return nodes_.count(); }
	bool is_connected(const Key& key) const { return nodes_.is_connected(key); }

	void shutdown()
	{
		for(iterator i = nodes_.begin(); i != nodes_.end(); ++i)
			i->close();
		nodes_.clear();
	}

protected:
	Nodes nodes_;

};

} // namespace g2

#endif // G2_NODESMANAGER_HPP


