#def reload_query home
#def reload_period $home_reload
#def reload_var home_reload

#include header.tpl

<h2 align=center>$full_name</h2>
<div align = center>
	<a href = "http://sharelin.sf.net">Home</a>&nbsp;&nbsp;&nbsp;
	<a href = "http://sourceforge.net/projects/sharelin/forums">Forums</a>&nbsp;&nbsp;&nbsp;
	<a href = "irc://irc.p2pchat.net:6667/sharelin">IRC</a>
</div>
<br>
<br>
#include linkform.tpl
	<br>
#if $version
	<table cellspacing = 5 cellpadding = 0 border = 0 align = center>
	<tr>
	<td><div align = center><b>New version available: <a href=$vlink>$version</a></b></div><br>$vcomments</td>
	</tr>
	</table>
#endif

	<table cellspacing = 0 cellpadding = 0 border = 0><tr>
	<td>
	<table><tr><td valign=top>
<h3>Status<br>======</h3>
	<div style = "margin-left:15;">

	<table cellspacing = 0 cellpadding = 0 border = 0>
		<tr><td>Downloads</td> <td>&nbsp;&nbsp;</td> <td><b>$total_downloads</b></td></tr>
		<tr><td>Uploads</td> <td>&nbsp;&nbsp;</td> <td><b>$total_uploads</b></td></tr>
		<tr><td>Searches</td> <td>&nbsp;&nbsp;</td> <td><b>$total_searches</b></td></tr>
		<tr><td>Average bw in</td> <td>&nbsp;&nbsp;</td> <td><b>$hr_avr_bw_in ps</b></td></tr>
		<tr><td>Average bw out</td> <td>&nbsp;&nbsp;</td> <td><b>$hr_avr_bw_out ps</b></td></tr>
		<tr><td>Address</td> <td>&nbsp;&nbsp;</td> <td><b>$address</b></td></tr>
		<tr><td>Firewalled</td> <td>&nbsp;&nbsp;</td> <td><b>$firewalled</b></td></tr>
		<tr><td>Uptime</td> <td>&nbsp;&nbsp;</td> <td><b>$uptime</b></td></tr>
	</table>
</div>
</td></tr></table>
	<br>
	<h3>Neighboring hubs<br>==============</h3>
	<table cellspacing = 0 cellpadding = 3 border = 0>

#foreach h_num h_address h_good_packets h_bad_packets h_sent_packets h_connected_time h_agent

#include fancydark.tpl
	<tr bgcolor=$color>
	<td>$h_num</td>
	<td width=10></td>

	<td>$h_address</td> 
	<td width=10></td>

	<td>$h_agent</td> 
	<td width=10></td>

	<td title = "Sent packets">$h_sent_packets</td>
      	<td title = "Received packets">$h_good_packets</td>
       	<td title = "Received bad packets">$h_bad_packets</td>
	<td width=10></td>
	<td title = "Time connected">$h_connected_time</td>

	</tr>
#next	

	</table>
#if $hs_num
<h3>Connecting to hosts<br>===============</h3>
	<table cellspacing = 0 cellpadding = 3 border = 0>

#foreach hs_num hs_address hs_connection_status

#include fancydark.tpl
	<tr bgcolor=$color>
	<td>$hs_num</td>
	<td width=10></td>

	<td>$hs_address</td> 
	<td width=10></td>

	<td width=10></td>
	<td>$hs_connection_status</td>

	</tr>
#next	
	</table>
#endif 

<br><br><a href = /kill>&gt;&gt;&gt; kill core &lt;&lt;&lt;</a>
	
#include footer.tpl

