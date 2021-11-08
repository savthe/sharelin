/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008  Andrey Stroganov <savthe@gmail.com>

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

#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>

#include "share_mgr.hpp"
#include "share/manager.hpp"
#include "share/localqht.hpp"
#include "share/storage.hpp"
#include "share/hasher.hpp"
#include "share/stored_file.hpp"
#include "querylang.hpp"
#include "app.hpp"
#include "share/tag.hpp"
#include "../verb.hpp"

using namespace share_details;

using boost::istarts_with;

ShareMgr::ShareMgr():
	impl_(new Manager)
{
}

ShareMgr::~ShareMgr()
{
	delete impl_;
}

void ShareMgr::shutdown()
{
	impl_->shutdown();
}

void ShareMgr::start()
{
	impl_->start();
}

void ShareMgr::tick()
{
	impl_->tick();
}

void ShareMgr::sync()
{
	impl_->sync();
}

void ShareMgr::updateSettings()
{
	impl_->updateSettings();
}

SharedFile ShareMgr::add(const FileInfo& i)
{
	return SharedFile(impl_->storage().add(i).get());
	/*
	boost::shared_ptr p(File(i));
	impl_->storage().add(p);

	//share::WeakInfo w = impl_->storage().add(f);
	impl_->qht().update(i);
	return SharedFile(p);
	*/
}

/*
void ShareMgr::update(const share::FileInfo& f)
{
	impl_->storage().update(f);
	impl_->qht().update(f);
}
*/


void ShareMgr::dump(Files& c)
{
	std::vector<StoredFile> v;
	impl_->storage().dump(std::back_inserter(v));
	BOOST_FOREACH(StoredFile& f, v)
		c.push_back(f.get());
}


SharedFile ShareMgr::findPath(const std::string& path)
{
	StoredFile f(impl_->storage().find<tag::path>(path));
	return f.get();
}

SharedFile ShareMgr::get(Id id)
{
	StoredFile f(impl_->storage().get<tag::id>(id));
	return f.get();
}

SharedFile ShareMgr::findHash(const hash::SHA1& h)
{
	StoredFile f(impl_->storage().find<tag::sha1>(h));
	return f.get();
}

SharedFile ShareMgr::findHash(const hash::TTR& h)
{
	StoredFile f(impl_->storage().find<tag::ttr>(h));
	return f.get();
}

SharedFile ShareMgr::findHash(const hash::MD5& h)
{
	StoredFile f(impl_->storage().find<tag::md5>(h));
	return f.get();
}

SharedFile ShareMgr::findHash(const hash::ED2K& h)
{
	StoredFile f(impl_->storage().find<tag::ed2k>(h));
	return f.get();
}

void ShareMgr::remove(Id id)
{
	impl_->storage().remove(id);
}

void ShareMgr::remove(const std::string& path)
{
	Id id = impl_->storage().find<tag::path>(path).id();
	impl_->storage().remove(id);
}

/*
void ShareMgr::finalize(Id id, const std::string& path)
{
	impl_->storage().rename(id, path);
	impl_->storage().finalize(id);
}
*/

SharedFile ShareMgr::find_urn(const std::string& urn)
{
	const std::size_t pos = urn.find_last_of( ':' );
	if( pos == std::string::npos ) return SharedFile();

	try
	{
		if( istarts_with( urn, "urn:sha1:" ) || istarts_with( urn, "sha1:" ) )
			return findHash( hash::SHA1( urn.substr( pos + 1 ) ) );

		if( istarts_with( urn, "urn:ttr:" ) || istarts_with( urn, "ttr:" ) ||
				istarts_with( urn, "urn:tree:tiger/:"))
			return findHash( hash::TTR( urn.substr( pos + 1 ) ) );

		if( istarts_with( urn, "urn:ed2k:" ) || istarts_with( urn, "ed2k:" ) )
			return findHash( hash::ED2K( urn.substr( pos + 1 ) ) );

		if( istarts_with( urn, "urn:md5:" ) || istarts_with( urn, "md5:" ) )
			return findHash( hash::MD5( urn.substr( pos + 1 ) ) );
	}
	catch(std::exception&)
	{
		verb1::shr << "Couldn't find urn: " << urn << std::endl;
	}

	return SharedFile();
}

/*
void ShareMgr::requested(share::WeakInfo i)
{
	impl_->storage().requested(i);
}

void ShareMgr::hit(share::WeakInfo i)
{
	impl_->storage().hit(i);
}
*/

SharedFile ShareMgr::findHashes(const hash::SHA1& sha1, const hash::TTR& ttr, const hash::ED2K& ed2k, hash::MD5& md5)
{
	SharedFile f;

	if (!f && !sha1.empty()) f = findHash(sha1);
	if (!f && !ttr.empty()) f = findHash(ttr);
	if (!f && !md5.empty()) f = findHash(md5);
	if (!f && !ed2k.empty()) f = findHash(ed2k);

	return f;
}

void ShareMgr::findDn(const std::string& dn, Files& c, std::size_t maxResults)
{
	std::set<std::string> include;
	std::set<std::string> exclude;

	G2::KeywordsFromQuery(dn, std::inserter(include, include.begin()), std::inserter(exclude, exclude.begin()));

	std::vector<StoredFile> v;
	impl_->storage().findDn(include.begin(), include.end(), exclude.begin(), exclude.end(), std::back_inserter(v), maxResults);
	BOOST_FOREACH(StoredFile& f, v)
		c.push_back(f.get());
}

const QueryHashTable& ShareMgr::qht()
{
	return impl_->qht().get();
}

std::time_t ShareMgr::qhtTime()
{
	return impl_->qht().timestamp();
}

