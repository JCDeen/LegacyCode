function removePointsPopup(query_string) {
	var hw = "toolbar=no,location=no,directories=no,status=no,scrollbars=yes,resizable=no,copyhistory=no,width=500,height=600";
	windowReference = window.open('remove_points_popup.php'+query_string, 'remove_points_popup', hw );
	if (!windowReference.opener) {
		windowReference.opener = self;
	}
	windowReference.focus();
}

function adjustPointsPopup(query_string) {
	var hw = "toolbar=no,location=no,directories=no,status=no,scrollbars=yes,resizable=no,copyhistory=no,width=500,height=600";
	windowReference = window.open('adjust_points_popup.php'+query_string, 'remove_points_popup', hw );
	if (!windowReference.opener) {
		windowReference.opener = self;
	}
	windowReference.focus();
}

// FOR player live search
var xmlHttp

function showResult(str,l)
{
	if (str.length==0)
	{
		document.getElementById("livesearch").innerHTML="";
		document.getElementById("livesearch").style.border="0px";
		return
	}

	xmlHttp=GetXmlHttpObject()

	if (xmlHttp==null)
	{
		alert ("Browser does not support HTTP Request")
		return
	}

	var url="livesearch.php"
	url=url+"?q="+str
	url=url+"&l="+l
	url=url+"&sid="+Math.random()
	xmlHttp.onreadystatechange=stateChanged
	xmlHttp.open("GET",url,true)
	xmlHttp.send(null)
}

function stateChanged()
{
	if (xmlHttp.readyState==4 || xmlHttp.readyState=="complete")
	{
		document.getElementById("livesearch").innerHTML=xmlHttp.responseText;
		document.getElementById("livesearch").style.border="1px solid #A5ACB2";
	}
}

function GetXmlHttpObject()
{
	var xmlHttp=null;
	try
	{
		// Firefox, Opera 8.0+, Safari
		xmlHttp=new XMLHttpRequest();
	}
	catch (e)
	{
		// Internet Explorer
		try
		{
			xmlHttp=new ActiveXObject("Msxml2.XMLHTTP");
		}
		catch (e)
		{
			xmlHttp=new ActiveXObject("Microsoft.XMLHTTP");
		}
	}
	return xmlHttp;
}

function addLoadEvent(func) {
	var oldonload = window.onload;
	if (typeof window.onload != 'function') {
		window.onload = func;
	} 
	else {
		window.onload = function() {
			if (oldonload) {
				oldonload();
			}
		func();
		}
	}
}

function blankFieldIfEqualsValue(field, value) {
	if(field.value == value) {
		field.value = '';
	}
}

function toggleVisibility(id) {
	var element = document.getElementById(id);
	if(element.style.display == 'block') {
		element.style.display = 'none';
	}
	else {
		element.style.display = 'block';
	}
}