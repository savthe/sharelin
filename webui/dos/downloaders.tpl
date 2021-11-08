<table border=0 cellpadding=0 cellspacing=0 ><tr><td width = 20></td><td>
<table border=0 cellpadding=0 cellspacing=0 >
#foreach dlr_address dlr_status dlr_pushed dlr_rate dlr_qnum dlr_qsize dlr_left dlr_right dlr_length dlr_total
	<tr>
		<td align = left> $dlr_address
			#if $dlr_pushed
				(<b>p</b>)
			#endif
		</td>
		
		<td width = 20></td>
		<td align = left>
			#if $dlr_status = init
				initializing
			#endif
	
			#if $dlr_status = idle
				thinking
			#endif

			#if $dlr_status = connect
				connecting
			#endif
			
			#if $dlr_status = reply
				talking to host
			#endif
			
			#if $dlr_status = tiger
				fetching tiger tree
			#endif

			#if $dlr_status = file
				<b>R $dlr_left  -- $dlr_right  ($dlr_length)</b> 
			#endif

			#if $dlr_status = queued
				Q $dlr_qnum  of $dlr_qsize
			#endif
		</td>
		
		<td width = 20></td>
		
		<td>
			#if $dlr_rate != 0 B
				$dlr_rate ps
			#endif
		</td>
		
		<td width = 20></td>
		
		<td>
			#if $dlr_total != 0 B
				Total in: $dlr_total
			#endif
		</td>
	</tr>
#next
</table>

</td></tr></table>
