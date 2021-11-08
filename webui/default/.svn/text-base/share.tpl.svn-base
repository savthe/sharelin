#def reload_query share
#def reload_period $share_reload
#def reload_var share_reload

#include header.tpl

<h2 align = center>Local share ($page_container_size  files)</h2>
<div align = center>
<a href = "/sync" target = "status">sync</a>
<br><br>
#foreach page_number
	#if $page_number = $page_current
		<b>$page_number</b>&nbsp;
	#else 
		<a href = /share?p=$page_number>$page_number</a>&nbsp;
	#endif 
#next
</div>
<br>
#foreach type name path size requests hits actual_size sha1 ed2k md5 ttr number
	<div id="info$number" class="xstooltip" align =left>
	<b>Size:</b> $actual_size  bytes<br>
	<b>Hits:</b> $hits<br>
	<b>Requests:</b> $requests<br>
	<br>
	<b>SHA1:</b> $sha1<br>
	<b>ED2K:</b> $ed2k<br>
	<b>TTR:</b> $ttr<br>
	<b>MD5:</b> $md5
	</div> 
#next 
<table align = center border = 0 cellpadding = 3 cellspacing = 0>
	<tr>
		<td></td>
		<td align = left><b><a href = "/share?sort=sfl&sortby=name">Name</a></b></td>
		<td width=10></td>
		<td align = right><b><a href = "/share?sort=sfl&sortby=size">Size</a></b></td>
		<td width=10></td>
		<td align = right><b><a href = "/share?sort=sfl&sortby=requests">Requests</a></b></td>
		<td width=10></td>
		<td align = right><b><a href = "/share?sort=sfl&sortby=hits">Hits</a></b></td>
	</tr>

#foreach type name path size requests hits number
	#include fancydark.tpl
	<tr bgcolor = $color>
		<td valign = middle><img src="/img/ext-$type.png"></td>
		<td align = left valign = middle>
		<span id="act$number" onmouseover="xstooltip_show('info$number', 'act$number', 0, 50);" onmouseout="xstooltip_hide('info$number');">
		$name
		<br>
	<div style="margin-left:15; font-size:11px;">
		$path
		</div>
		</span>
</td>
		<td width=10></td>
		<td align = right>
#if $size = 0 B
	not scanned yet
#else
<nobr>	$size</nobr>
#endif
		</td>
		<td width=10></td>
		<td align = right>$requests</td>
		<td width=10></td>
		<td align = right>$hits</td>
	</tr>
#next
</table>
<br> <br>
<div align = center>
#foreach page_number
	#if $page_number = $page_current
		<b>$page_number</b>&nbsp;
	#else 
		<a href = /share?p=$page_number>$page_number</a>&nbsp;
	#endif 

#next
</div>

#include footer.tpl
