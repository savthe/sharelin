<table cellspacing = 0 align = center cellpadding = 0 border = 0>
<tr>
	<td align = center valign = bottom>
		<a href = "/home">
			<img src = "img/overview.png" border = 0>

#if $query = home 
	<p class ="hl"><b>Home</b></p>
#else
	<p class = "nohl"><b>Home</b></p>
#endif
	
		</a>
	</td>
	
	<td width = 20></td>
		
	<td align = center valign = bottom>
		<a href = "/dls">
			<img src = "img/download.png" border = 0>
#if $query = dls
	<p class ="hl"><b>Downloads</b></p>
#else
	<p class = "nohl"><b>Downloads</b></p>
#endif
		</a>
	</td>
	
	<td width = 20></td>
	
	<td align = center valign = bottom>
		<a href = "/ups">
			<img src = "img/upload.png" border = 0>
#if $query = ups 
	<p class ="hl"><b>Uploads</b></p>
#else
	<p class = "nohl"><b>Uploads</b></p>
#endif
		</a>
	</td>
	
	<td width = 20></td>
		
	<td align = center valign = bottom>
		<a href = "/srs">
			<img src = "img/search.png" border = 0>
#if $query = srs
	<p class ="hl"><b>Search</b></p>
#else
	<p class = "nohl"><b>Search</b></p>
#endif
		</a>
	</td>
		
	<td width = 20></td>
		
	<td align = center valign = bottom>
		<a href = "/incoming">
			<img src = "img/folder.png" border = 0>
#if $query = incoming
	<p class ="hl"><b>Incoming</b></p>
#else
	<p class = "nohl"><b>Incoming</b></p>
#endif
		</a>
	</td>
		
	<td width = 20></td>
		
	<td align = center valign = bottom>
		<a href = "/settings">
			<img src = "img/settings.png" border = 0>
#if $query = settings
	<p class ="hl"><b>Settings</b></p>
#else
	<p class = "nohl"><b>Settings</b></p>
#endif
		</a>
	</td>
		
	<td width = 20></td>
		
	<td align = center valign = bottom>
		<a href = "/log">
		<img src = "img/log.png" border = 0>
#if $query = log
	<p class ="hl"><b>View log</b></p>
#else
	<p class = "nohl"><b>View log</b></p>
#endif
		</a>
	</td>
	
	<td width = 20></td>
		
	<td align = center valign = bottom>
		<a href = "/share">
		<img src = "img/share.png" border = 0>
#if $query = share
	<p class ="hl"><b>Share</b></p>
#else
	<p class = "nohl"><b>Share</b></p>
#endif
		</a>
	</td>
</tr>
</table>
