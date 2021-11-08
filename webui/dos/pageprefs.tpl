#if $reload_var
<script language="javascript"> 
function ShowPrefs(link) 
{
	if(typeof ShowPrefs.shown == 'undefined')
	{
		ShowPrefs.shown = false;
	}

	var elPrefs = document.getElementById("prefs");

	ShowPrefs.shown = !ShowPrefs.shown;
	if(ShowPrefs.shown) 
	{
		elPrefs.style.display = "block";
		link.innerHTML = "&lt;"
	}
	else 
	{
		elPrefs.style.display = "none";
		link.innerHTML = "&gt;"
	}
} 
</script>

<div align = left>
<a id = "aPagePrefs" style="font-size: 12px; text-decoration: none;" href="#" 
onclick="javascript: ShowPrefs(this); return false">&gt;</a> 
</div>

<div id="prefs" style ="width=auto; height=230px;  margin-top:0px; display: none">
Reload every <br>

#if $reload_period = 0
<b>
#endif
<a href = "/uiset?$reload_var=0&redirect=$reload_query">off</a>
#if $reload_period = 0
</b>
#endif
&nbsp;

#if $reload_period = 10
<b>
#endif
<a href = "/uiset?$reload_var=10&redirect=$reload_query">10s</a>
#if $reload_period = 10
</b>
#endif
&nbsp;

#if $reload_period = 30
<b>
#endif
<a href = "/uiset?$reload_var=30&redirect=$reload_query">30s</a>
#if $reload_period = 30
</b>
#endif
&nbsp;

#if $reload_period = 60
<b>
#endif
<a href = "/uiset?$reload_var=60&redirect=$reload_query">1m</a>
#if $reload_period = 60
</b>
#endif
&nbsp;

#if $reload_period = 300
<b>
#endif
<a href = "/uiset?$reload_var=300&redirect=$reload_query">5m</a>
#if $reload_period = 300
</b>
#endif
&nbsp;

#if $reload_period = 1800
<b>
#endif
<a href = "/uiset?$reload_var=1800&redirect=$reload_query">30m</a>
#if $reload_period = 1800
</b>
#endif

</div>

#endif

