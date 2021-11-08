link_directories ( ${Boost_LIBRARY_DIRS} )

set (SHARELIN_SRC
		application.cpp	
		app.cpp	
		appdefs
		async_work.cpp
		async_task_server.cpp
		asynchash.cpp
		asynccopy.cpp
		asyncfile.cpp

		contenttype.cpp
		configurer.cpp

		directorywalk.cpp
		downloads.cpp
		download_mgr.cpp

		filesystem
		file
		filemap

		g2packet
		g2tools
		gatereactor
		gate.cpp
		gate_mgr.cpp
		g2net_mgr.cpp
		g2net.cpp
		g2queueinfo
		gatekeeper_impl

		history.cpp
		history_mgr.cpp
		httpant

		integrity

		logger

		main.cpp

		queryhashtable.cpp

		sourcehost
		streamlog
		security.cpp
		security_mgr.cpp
		settings.cpp
		settings_control.cpp
		settings_mgr.cpp
		sourcehost
		share.cpp
		share_mgr.cpp
		searches.cpp
		search_mgr.cpp
		searcher.cpp
		sharedfile.cpp

		task_server.cpp
		trafmanager.cpp
		traf_client.cpp
		traf.cpp

		updateswatch
		uploadbuffer
		uploadfilebuffer
		uploads.cpp
		upload_mgr.cpp
		ui_mgr.cpp

		verb

		async_task/task.cpp
		async_task/server.cpp
		async_task/thread_pool.cpp

		upload/manager.cpp
		upload/uploader.cpp
		upload/file_send_buffer.cpp
		upload/bytes_send_buffer.cpp
		upload/uploader_state_file.cpp
		upload/uploader_state_tiger.cpp
    )

set (G2NET_SRC
		g2net/packethandler
		g2net/hubcache
		g2net/hubs
		g2net/g2packetsender
		g2net/g2packetreceiver
		g2net/node_impl
		g2net/g2sequencesender
		g2net/transceiver
		g2net/arrivalcache
		g2net/departurecache
		g2net/udp
		g2net/connector_impl
		g2net/manager
		g2net/discovery
		g2net/nodeinfo
		g2net/probe
		g2net/connectorsmanager
    )

set (SHARE_SRC
		share/manager
		share/cache
		share/localqht
		share/storage
		share/hasher
		share/stored_file.cpp
		share/syncer.cpp
		share/tools.cpp
    )

set (SEARCH_SRC
		search/manager.cpp
		search/defs
		search/freshhubs
		search/searchedhubs
		search/holder.cpp
		search/queue.cpp
		search/keykeeper
		search/keygather
		search/search
		search/neighbors
		search/tools
		search/searcher_impl.cpp
		search/user_search.cpp
    )

set(COMMON_SRC
		common/range
		common/segment
		common/ipsegment
		common/buffer
		common/util
		common/types
		common/staticname
		common/debug
		common/ip
		common/media
		common/env
		common/guid
		common/zlib
		common/period
		common/url
		common/increment
		common/idgen
		common/io.cpp
		common/net_err.cpp
		common/tigertree.cpp
		common/tiger_tree_base.cpp
		common/tiger_tree_node.cpp
		common/tiger_tree_composer.cpp
		common/hash/ed2khash
		common/hash/hash
		common/hash/sha1
		common/hash/ttr
		common/hash/md5
		common/hash/sboxes
		common/hash/tiger
		common/conv/hex
		common/conv/conv
		common/conv/base32
		common/conv/common
		common/ticker.cpp
		common/ticker_core.cpp
)

set (G2PACKET_SRC
		g2packet/util
		g2packet/packet
		g2packet/packettype
		g2packet/reader
		g2packet/writer
		g2packet/header
    )

set (FIO_SRC
		fio/util
		fio/common
    )
set (UI_SRC
		ui/common/query
		ui/common/tag
		ui/common/template
		ui/common/sortprefs
		ui/common/server
		ui/common/default_reactor.cpp
		ui/common/comparers.cpp
		ui/common/tools.cpp
		ui/common/reactor.cpp
		ui/common/searches.cpp
		ui/telnet/client.cpp
		ui/telnet/server.cpp
		ui/telnet/tools.cpp
		ui/telnet/reactor.cpp
		ui/web/reactor.cpp
		ui/web/client.cpp
		ui/web/server.cpp
    )

set (SETTINGS_SRC
    )

set (DOWNLOAD_SRC
		download/download.cpp
		download/manager.cpp
		download/downloader.cpp
		download/bytes_receive_buffer.cpp
		download/file_receive_buffer.cpp
		download/range_destribute.cpp
		download/fetcher.cpp
		download/sources.cpp
		download/connector.cpp
		download/validator.cpp
		download/transfer.cpp
		download/tools.cpp
    )

add_executable (sharelin 
		${SHARELIN_SRC} 
		${G2NET_SRC} 
		${DOWNLOAD_SRC} 
		${SHARE_SRC} 
		${SEARCH_SRC} 
		${COMMON_SRC} 
		${G2PACKET_SRC} 
		${FIO_SRC} 
		${UI_SRC} 
		${SETTINGS_SRC}
		)

target_link_libraries ( sharelin 
		${Boost_THREAD_LIBRARY} 
		${Boost_IOSTREAMS_LIBRARY} 
		${Boost_FILESYSTEM_LIBRARY} 
		${Boost_SYSTEM_LIBRARY} 
		${ZLIB_LIBRARIES}
		)

