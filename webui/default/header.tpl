<html>
	<head>
		<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
#if $reload_period
		<meta http-equiv="refresh"  content="$reload_period">
#endif 
 		<link rel="icon" type="image/png" href="/img/fav.png" />
		<title>Sharelin</title>
		<style type="text/css">
#body {overflow-y:scroll;} 
a
{
	text-decoration: none;
	color: #FF6600;
}
a:active
{
	text-decoration: none;
	color: #FF6600;
}
a:visited
{
	text-decoration: none;
	color: #FF6600;
}

a:hover
{
	text-decoration: underline;
	color: #FF6600;
}

table {font-size:12px;}

.texta {
	font-size: 12px;
	background-color: #DDDDDD;
	border: 1px solid #666666;
}

.button {
	border: 1px solid;
}

.tinyform {
	margin: 0px; padding: 0px;
}

.hl {
	background-color: #e8e8e8;
	padding-top: 5px;
	padding-right: 5px;
	padding-bottom: 5px;
	padding-left: 5px;
}

.nohl {
	background-color: #ffffff;
	padding-top: 5px;
	padding-right: 5px;
	padding-bottom: 5px;
	padding-left: 5px;
}
.xstooltip 
{
	visibility: hidden; 
	position: absolute; 
        top: 0;  
        left: 0; 
	z-index: 2; 
	background-repeat: repeat;
	color: #ffffff;
	background-image: url("/img/xst.png");

        font: normal 8pt sans-serif; 
        padding: 3px; 
        border: solid 1px;
}
		</style>
	</head>
	<body
#if $query = search
	onload = "document.search.dn.focus();"
#endif
	>
<script language="javascript">
/*
   xstooltip functions are tagen from this guide: 
   http://www.texsoft.it/index.php?c=software&m=sw.js.htmltooltip&l=it
 */
function xstooltip_findPosX(obj) 
{
	var curleft = 0;
	if (obj.offsetParent) 
	{
		while (obj.offsetParent) 
		{
			curleft += obj.offsetLeft;
			obj = obj.offsetParent;
		}
	}
	else if (obj.x)
		curleft += obj.x;
	return curleft;
}

function xstooltip_findPosY(obj) 
{
	var curtop = 0;
	if (obj.offsetParent) 
	{
		while (obj.offsetParent) 
		{
			curtop += obj.offsetTop;
		        obj = obj.offsetParent;
		}
	}
	else if (obj.y)
		curtop += obj.y;
	return curtop;
}

function xstooltip_show(tooltipId, parentId, posX, posY)
{
	it = document.getElementById(tooltipId);
	        
	if ((it.style.top == '' || it.style.top == 0) && (it.style.left == '' || it.style.left == 0))
	{
		it.style.width = it.offsetWidth + 'px';
	        it.style.height = it.offsetHeight + 'px';
				            
	        img = document.getElementById(parentId); 
							        
	        if (posX + it.offsetWidth > img.offsetWidth) posX = img.offsetWidth - it.offsetWidth;
	        if (posX < 0 ) posX = 0; 
									            
	        x = xstooltip_findPosX(img) + posX;
	        y = xstooltip_findPosY(img) + posY;
	        it.style.top = y + 'px';
	        it.style.left = x + 'px';
	}
		    
	it.style.visibility = 'visible'; 
}

function xstooltip_hide(id)
{
	it = document.getElementById(id); 
	it.style.visibility = 'hidden'; 
}
</script>
	<table cellspacing = 0 cellpadding = 0 width = 100%>
		<tr><td>
			#include menu.tpl
			#include statusbar.tpl
		</td>
		<td valign=top>
			#include pageprefs.tpl
		</td>
		</tr>
	</table>
	<table cellspacing = 0 cellpadding = 0 width = 100%>
		<tr><td><br>
		<iframe name = "status" frameborder = 0 width=100% height=50 align="center"> </iframe>		
		<br></td></tr>
		<tr><td>
		<table cellspacing=0 cellpadding=0 width=80% align=center border = 0>
			<tr width = 100%>
				<td valign = top>
#if $status
				<div align = center>$status</div>
				<br>
#endif
