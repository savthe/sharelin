#def reload_query dls
#def reload_period $dls_reload
#def reload_var dls_reload

#include header.tpl
<table align = center cellspacing=0 cellpadding = 0 border=0><tr><td>

<h2 align = center>Downloads ($total_downloads)</h2>
#if $total_downloads
	#if $webui_expand_downloads
		<div align = center><a href = "/dls?expand=0">collapse</a></div>
	#else
		<div align = center><a href = "/dls?expand=1">expand</a></div>
	#endif
	<form method=POST action="/dlgroup">	
	<table width=100% border=0 cellpadding=2 cellspacing=0 >
	<tr bgcolor = "#FFFFFF">
		<td></td>
		<td align = left><b><a href=/dls?sort=adl&sortby=name>Name</a></b></td>

		<td width=10></td>
		<td align=right><b><a href=/dls?sort=adl&sortby=size>Size</a></b></td>
			
		<td width=10></td>
		<td align=right><b><a href=/dls?sort=adl&sortby=done>Done&nbsp;%</a></b></td>
		
		<td width=10></td>
		<td align=center><b><a href=/dls?sort=adl&sortby=rate>Rate</a></b></td>
		
		<td width=10></td>
		<td align=right><b><a href=/dls?sort=adl&sortby=sources>Sources</a></b></td>

		<td width=10></td>
		<td align=center><b><a href=/dls?sort=adl&sortby=priority>Priority</a></b></td>

	</tr>

	#foreach download_id download_name download_size dl_status progress download_rate active_sources priority sources downloaders
		#if $webui_expand_downloads
			#def color #dfdfdf
		#else
			#include fancydark.tpl
		#endif 
		<tr bgcolor = $color>
			<td valign = middle>
			<nobr>
				<input type="checkbox" name="id$download_id">&nbsp;&nbsp;&nbsp;&nbsp;
				<a href="/dldel?id=$download_id"><img border = 0 title="Remove download" src=img/delete16.png></a>&nbsp;&nbsp;
				#if $dl_status = paused
					<a href="/dlresume?id=$download_id"><img border = 0 title="Resume download" src=img/resume16.png></a>
				#else
					<a href="/dlstop?id=$download_id"><img border = 0 title="Pause download" src=img/stop16.png></a>
				#endif
			</nobr>
			</td>
		
			<td align = left><a href = "/dlinfo?id=$download_id">$download_name</a></td>
			<td width=10></td>

			<td align=right><nobr>$download_size</nobr></td>
			<td width=10></td>

			<td align=right><span title="Status: $dl_status"><b>
			#if $progress
				$progress
			#endif
			</b></span></td>
			<td width=10></td>

			<td align=right><nobr>
#if $dl_status = transfer
#if $download_rate != 0 B
			$download_rate
#endif
#endif 
#if $dl_status = queued
			queued
#endif
#if $dl_status = paused
			paused
#endif
			</nobr></td>
			<td width=10></td>

			<td align=right><span title="Known: $sources">
#if $active_sources
			$active_sources
#endif 
			</span></td>
			<td width=10></td>

			<td align=center>
			<span title = "Low">
#if $priority = -2
			<b>L</b>
#else
			<a href="/dlpri?id=$download_id&priority=-2">L</a>
#endif
			</span>
			&nbsp;
			<span title = "Below normal">
#if $priority = -1
			<b>B</b>
#else
			<a href="/dlpri?id=$download_id&priority=-1">B</a>
#endif
			</span>
			&nbsp;
			<span title = "Normal">
#if $priority = 0
			<b>N</b>
#else
			<a href="/dlpri?id=$download_id&priority=0">N</a>
#endif
			</span>
			&nbsp;
			<span title = "Above normal">
#if $priority = 1
			<b>A</b>
#else
			<a href="/dlpri?id=$download_id&priority=1">A</a>
#endif
			</span>
			&nbsp;
			<span title = "High">
#if $priority = 2
			<b>H</b>
#else
			<a href="/dlpri?id=$download_id&priority=2">H</a>
#endif
			</span>
			</td>
			<td width=10></td>
		</tr>

#if $webui_expand_downloads
		#if $downloaders
			<tr>
				<td></td>
				<td colspan=11>$downloaders</td>
			</tr>
		#endif
#endif 

#next
	</table>
	<br><br>
	<select name="selected" class="texta" onchange='this.form.submit()'>
		<option value="nothing">Apply to selected</option>
		<option value="pause">Pause</option>
		<option value="resume">Resume</option>
		<option value="remove">Remove</option>
		<option value="priorityl">Priority: LOW</option>
		<option value="priorityb">Priority: BELOW NORMAL</option>
		<option value="priorityn">Priority: NORMAL</option>
		<option value="prioritya">Priority: ABOVE NORMAL</option>
		<option value="priorityh">Priority: HIGH</option>
																						</select>
																																
	</form>
	<br>
#endif

<h2 align=center>Downloads history</h2>
<div align = center><a href = "?purge">clear</a></div>
<table align = center border = 0 cellpadding = 3 cellspacing = 0>
#foreach dlhist_type dlhist_name dlhist_size
	#if $color = #dfdfdf
		#def color #fafafa
	#else
		#def color #dfdfdf
	#endif
	<tr bgcolor = $color>
		<td valign = middle> <img src="/img/ext-$dlhist_type.png"></td>
		<td align = left valign = middle>
			<a href = "/fetch/$dlhist_name">$dlhist_name</a>
			<br>
			&nbsp;&nbsp;&nbsp;
			<b>$dlhist_size</b>
		</td>
	</tr>
#next
</table>

</td></tr></table>

#include footer.tpl
