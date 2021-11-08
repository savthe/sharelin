#def reload_query srs
#def reload_period $srs_reload
#def reload_var srs_reload

#include header.tpl
#include searchform.tpl

#if $total_searches
<h2>Searches ($total_searches)<br>=============</h2>
	<div style = "margin-left:15;">
<table border=0 cellpadding=3 cellspacing=0 >
	<tr>
		<td></td>
		<td><b>Criteria</b></td>
		<td width=10></td>
		
		<td align=right><b>Results</b></td>
	</tr>
#foreach id extensions criteria hits
	<tr>
		<td>
			[<a href="/srdel?id=$id">X</a>]
		</td>

		<td><a href="/srshow?id=$id">$criteria</a>
			#if $extensions 
				<br><b>$extensions</b>
			#endif
				</td>
		<td width=10></td>
		
		<td align=right>$hits</td>

	</tr>
#next
</table>
</div>
#endif

#include footer.tpl

