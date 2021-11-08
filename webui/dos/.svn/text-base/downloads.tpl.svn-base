#def reload_query dls
#def reload_period $dls_reload
#def reload_var dls_reload

#include header.tpl

<table cellspacing=0 cellpadding = 0 border=0><tr><td>

<h2>Downloads ($total_downloads)<br>=============</h2>
#if $total_downloads
	#if $webui_expand_downloads
		<div align = center> <a href = "/dls?expand=0">(collapse)</a></div>
	#else
		<div align = center> <a href = "/dls?expand=1">(expand)</a></div>
	#endif
	<br>
	<div style = "margin-left:15;">
		
	<form method=POST action="/dlgroup">
	<table width=100% border=0 cellpadding=3 cellspacing=0 >
	<tr>
		<td></td>
		<td align = left><b><a href=/dls?sort=adl&sortby=name>Name</a></b></td>

		<td width=10></td>
		<td align=right><b><a href=/dls?sort=adl&sortby=size>Size</a></b></td>
			
		<td width=10></td>
		<td align=right><b><a href=/dls?sort=adl&sortby=done>Done %</a></b></td>
		
		<td width=10></td>
		<td align=center><b><a href=/dls?sort=adl&sortby=rate>Rate</a></b></td>
		
		<td width=10></td>
		<td align=right><b><a href=/dls?sort=adl&sortby=sources>Sources</a></b></td>

		<td width=10></td>
		<td align=center><b>Status</b></td>

		<td width=10></td>
		<td align=center><b><a href=/dls?sort=adl&sortby=priority>Priority</a></b></td>
	</tr>

	#foreach download_id download_name download_size dl_status progress download_rate active_sources priority sources downloaders
		#def color #000000
		<tr bgcolor = "$color">
			<td>
				<input type="checkbox" name="id$download_id">&nbsp;&nbsp;&nbsp;&nbsp;
				[<a title = "Remove download" href="/dldel?id=$download_id">X</a>]&nbsp;
				#if $dl_status = paused
					[<a title = "Resume download" href="/dlresume?id=$download_id">R</a>]
				#else
					[<a title = "Pause download" href="/dlstop?id=$download_id">P</a>]
				#endif
			</td>
		
			<td align = left><a href = "/dlinfo?id=$download_id">$download_name</a></td>
			<td width=10></td>

			<td align=right>$download_size</td>
			<td width=10></td>

			<td align=right><b>
			#if $progress
				$progress
			#endif 
			</b></td>
			<td width=10></td>

			<td align=right>
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
			</td>
			<td width=10></td>

			<td align=right>$active_sources  ($sources)</td>
			<td width=10></td>

			<td align=center>$dl_status</td>
			<td width=10></td>

			<td align=center>
			<span title = "Low">
#if $priority = -2 
			<b>L</b>
#else
			<a href="/dlpri?id=$download_id&priority=-2">l</a>
#endif
			</span>
			&nbsp;
			<span title = "Below normal">
#if $priority = -1
			<b>B</b>
#else
			<a href="/dlpri?id=$download_id&priority=-1">b</a>
#endif
			</span>
			&nbsp;
			<span title = "Normal">
#if $priority = 0
			<b>N</b>
#else
			<a href="/dlpri?id=$download_id&priority=0">n</a>
#endif
			</span>
			&nbsp;
			<span title = "Above normal">
#if $priority = 1
			<b>A</b>
#else
			<a href="/dlpri?id=$download_id&priority=1">a</a>
#endif
			</span>
			&nbsp;
			<span title = "High">
#if $priority = 2
			<b>H</b>
#else
			<a href="/dlpri?id=$download_id&priority=2">h</a>
#endif
			</span>
			</td>
			<td width=10></td>
		</tr>

#if $webui_expand_downloads
		#if $downloaders
			<tr>
				<td></td>
				<td>$downloaders</td>
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
	</div>
#endif

<h2>Download history (<a href = "?purge">X</a>)<br>===================</h2>
	<div style = "margin-left:15;">
<table border = 0 cellpadding = 3 cellspacing = 0>
#foreach dlhist_type dlhist_name dlhist_size
	<tr>
		<td align = left valign = middle>
			<a href = "/fetch/$dlhist_name">$dlhist_name</a>
			<br>
			&nbsp;&nbsp;&nbsp;
			<b>$dlhist_size</b>
		</td>
	</tr>
#next
</table>

</div>
</td></tr></table>

#include footer.tpl

