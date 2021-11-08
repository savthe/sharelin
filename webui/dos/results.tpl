#def reload_query srshow%3fid%3d$id
#def reload_period $srshow_reload
#def reload_var srshow_reload

#include header.tpl
#include searchform.tpl

<h2>Search results<br>====================</h2>
<div align = center>Name: <b>$dn</b>
#if $min
	&nbsp;&nbsp;&nbsp;Min: <b>$min</b>
#endif
#if $max
	&nbsp;&nbsp;&nbsp;Max: <b>$max</b>
#endif
<br>	
#if $extensions
	File types: <b>$extensions</b>
#endif
<br>	<br>
	<div align = center><a href = /srshow?id=$id>refresh</a></div>
	<br>
#foreach pagen
	#if $pagen = $page_cur
		<b>$page_number</b>&ensp;
	#else 
		<a href = /srshow?id=$id&p=$pagen_number>$page_number</a>&ensp;
	#endif 

#next
<br>
#foreach sha1 ttr ed2k md5 sources number rate actual_size
	<div id="info$number" class="xstooltip" align =left>
	<b>Size:</b> $actual_size  bytes<br>
	<b>Sources:</b> $sources<br>
	<b>Rate:</b> $rate ps
	<br><br>
	<b>SHA1:</b> $sha1<br>
	<b>ED2K:</b> $ed2k<br>
	<b>TTR:</b> $ttr<br>
	<b>MD5:</b> $md5
	</div> 
#next
<div style = "margin-left:15;">
<table align = center border=0 cellpadding=3 cellspacing=0 >
	<tr>
		<td><b><a href=/srshow?id=$id&sort=sres&sortby=name>Name</a></b></td>
		<td width=10></td>
		
		<td align=right><b><a href=/srshow?id=$id&sort=sres&sortby=size>Size</a></b></td>
		<td width=10></td>
		
		<td align=right><b><a href=/srshow?id=$id&sort=sres&sortby=sources>Sources</a></b></td>
	</tr>
#foreach type name size sources number rate
	<tr>
		<td>
		<span id="act$number" onmouseover="xstooltip_show('info$number', 'act$number', 0, 50);" onmouseout="xstooltip_hide('info$number');">
		$name
		</span>
		</td>
		<td width=10></td>
		
		<td align=right><nobr>$size</nobr></td>
		<td width=10></td>
		
		<td align=right><b>$sources</b><br><span class="tiny_hint">$rate ps</span></td>
	</tr>
#next
</table>
#foreach pagen
	#if $pagen = $page_cur
		<b>$page_number</b>&ensp;
	#else 
		<a href = /srshow?id=$id&p=$page_number>$page_number</a>&ensp;
	#endif 

#next
</div>
<br><br>

#include footer.tpl

