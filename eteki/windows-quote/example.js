/*
 * Adapted from code found in the book "Core HTML5 Canvas", 
 * Copyright (C) 2012 David Geary.
 */

var canvas = document.getElementById('canvas'), context = canvas.getContext('2d'),

    resetButton = document.getElementById('resetButton'),
    uploadButton = document.getElementById('uploadButton'),

    image = new Image(),
    imageData,
    imageDataCopy = context.createImageData(canvas.width, canvas.height),

    mousedown = {},
    rubberbandRectangle = {},
    dragging = false,

	markerCounter = 0,
	markersDrawn = [], // NEED TO SET THIS TO DATA TYPE EXPLICITLY ????
	grids = [],
	windowType = [];

	testData = [];
	
	let markers = [];
	let pauseMarkerCounter = false;

// Functions.....................................................

function windowToCanvas(canvas, x, y) 
{
   var canvasRectangle = canvas.getBoundingClientRect();
   
   return {
             x: x - canvasRectangle.left,
             y: y - canvasRectangle.top
          };
}

function copyCanvasPixels() 
{
   var i=0;
   
   console.log('captureCanvasPixels');
   
   // Copy red, green, and blue components of the first pixel
   for (i=0; i < 3; i++) 
   {
      imageDataCopy.data[i] = imageData.data[i];
   }

   // Starting with the alpha component of the first pixel,
   // copy imageData, and make the copy more transparent
   for (i=3; i < imageData.data.length - 4; i+=4) 
   {
      imageDataCopy.data[i]   = imageData.data[i] / 2; // Alpha: more transparent
      imageDataCopy.data[i+1] = imageData.data[i+1]; // Red
      imageDataCopy.data[i+2] = imageData.data[i+2]; // Green
      imageDataCopy.data[i+3] = imageData.data[i+3]; // Blue
   }
}

function captureCanvasPixels() 
{
   console.log('captureCanvasPixels');
   imageData = context.getImageData(0, 0, canvas.width, canvas.height);
   copyCanvasPixels();
}

function restoreRubberbandPixels() 
{
   var deviceWidthOverCSSPixels = imageData.width / canvas.width,
       deviceHeightOverCSSPixels = imageData.height / canvas.height;

   // Restore the Canvas to what it looked like when the mouse went down
   context.putImageData(imageData, 0, 0);

   // Put the more transparent image data into the rubberband rectangle
   context.putImageData(imageDataCopy, 0, 0,
      (rubberbandRectangle.left + context.lineWidth),
      (rubberbandRectangle.top + context.lineWidth),
      (rubberbandRectangle.width - 2*context.lineWidth) * deviceWidthOverCSSPixels,
      (rubberbandRectangle.height - 2*context.lineWidth) * deviceHeightOverCSSPixels);
}

// JCDeen
function pixelReset() 
{
   var deviceWidthOverCSSPixels = imageDataCopy.width / canvas.width,
       deviceHeightOverCSSPixels = imageDataCopy.height / canvas.height;

   // Restore the Canvas to what it looked like when the mouse went down
   /////context.putImageData(imageData, 0, 0);

   // Put the more transparent image data into the rubberband rectangle
   context.putImageData(imageDataCopy, 0, 0,
      (rubberbandRectangle.left + context.lineWidth),
      (rubberbandRectangle.top + context.lineWidth),
      (rubberbandRectangle.width - 2*context.lineWidth) * deviceWidthOverCSSPixels,
      (rubberbandRectangle.height - 2*context.lineWidth) * deviceHeightOverCSSPixels);
}

function setRubberbandRectangle(x, y) 
{
   rubberbandRectangle.left = Math.min(x, mousedown.x);
   rubberbandRectangle.top = Math.min(y, mousedown.y);
   rubberbandRectangle.width = Math.abs(x - mousedown.x),
   rubberbandRectangle.height = Math.abs(y - mousedown.y);

   //console.log('setRubberbandRectangle ' + x +','+y);
}

function drawRubberband() 
{
   var deviceWidthOverCSSPixels = imageData.width / canvas.width,
       deviceHeightOverCSSPixels = imageData.height / canvas.height;

   context.strokeRect(rubberbandRectangle.left + context.lineWidth,
                      rubberbandRectangle.top + context.lineWidth,
                      rubberbandRectangle.width - 2*context.lineWidth,
                      rubberbandRectangle.height - 2*context.lineWidth);
}

function rubberbandStart(x, y) 
{
   mousedown.x = x;
   mousedown.y = y;

   rubberbandRectangle.left = mousedown.x;
   rubberbandRectangle.top = mousedown.y;
   rubberbandRectangle.width = 0;
   rubberbandRectangle.height = 0;

   //console.log('rubberbandStart ' + x +','+y);
   dragging = true;

   captureCanvasPixels();
}

function rubberbandStretch(x, y) 
{
   //console.log('rubberbandStretch');
   if (rubberbandRectangle.width > 2*context.lineWidth &&
       rubberbandRectangle.height > 2*context.lineWidth) {
      if (imageData !== undefined) {
         restoreRubberbandPixels();
      }
   }

   setRubberbandRectangle(x, y);

   if (rubberbandRectangle.width > 2*context.lineWidth &&
       rubberbandRectangle.height > 2*context.lineWidth) 
   {
      drawRubberband();
   }
};

/* extensive modification by JCDeen. */
function rubberbandEnd() 
{
   if ((rubberbandRectangle.width > 0) && (rubberbandRectangle.height > 0))
   {
	   console.log('rubberbandEnd ' + markerCounter);
	   
	   drawRubberband();
	   
	   markersDrawn[markerCounter] = rubberbandRectangle.left + ',' + rubberbandRectangle.top + ','
	   	+ rubberbandRectangle.width + ',' + rubberbandRectangle.height;
	   
	   markers[markerCounter] = '{"left":' + rubberbandRectangle.left
	   	+ ',"top":' + rubberbandRectangle.top + ',"height":' + rubberbandRectangle.height 
	   	+ ',"width":' + rubberbandRectangle.width +'}';
	   
	   console.log('markersDrawn[' + markerCounter + ']=' + markersDrawn[markerCounter]);
	   
	   var oldFillStyle = context.fillStyle;
	   context.fillStyle = 'White';
	   context.fillRect(rubberbandRectangle.left + 2, rubberbandRectangle.top + 2, 25, 25 );
	   context.fillStyle = oldFillStyle;
	   context.fillText(markerCounter, rubberbandRectangle.left + 5, rubberbandRectangle.top + 24 );
	   
	   var dataform = document.getElementById("upload_form");
	   var markersTable = document.getElementById("markersTable");
	   var row = markersTable.insertRow();
	   
	   row.innerHTML = '<td>' + markerCounter + '</td>' + 
		'<td><input name="grids' + markerCounter + '" type="radio" value="upper" onclick="CheckGrids('+ markerCounter + ', \'upper\');">Upper ' + 
		    '<input name="grids' + markerCounter + '" type="radio" value="lower" onclick="CheckGrids('+ markerCounter + ', \'lower\');">Lower ' + 
		    '<input name="grids' + markerCounter + '" type="radio" value="both" onclick="CheckGrids('+ markerCounter + ', \'both\');" CHECKED>Both ' + 
		    '<input name="grids' + markerCounter + '" type="radio" value="none" onclick="CheckGrids('+ markerCounter + ', \'none\');" >None </td>' + 
		    
		    '<td><input name="window_type' + markerCounter + '" type="radio" value="double hung" CHECKED onclick="CheckWindowType('+ markerCounter + ', \'double hung\');">Double Hung ' + 
		        '<input name="window_type' + markerCounter + '" type="radio" value="picture" onclick="CheckWindowType('+ markerCounter + ', \'picture\');">Picture </td>' +
		        
		    '<td><input type="button" value="Remove Marker" onclick="RemoveMarker('+ markerCounter +');"/></td>';
	   row.id ="row"+ markerCounter;
	
	   // Default values.
	   grids[markerCounter]='both;'
	   windowType[markerCounter]='double hung';
	   markerCounter++;
   }
   
   dragging = false;
   if (!pauseMarkerCounter)
	   imageData = undefined;
}

// Mouse Event handlers...............................................

canvas.onmousedown = function (e) 
{ 
   var loc = windowToCanvas(canvas, e.clientX, e.clientY);
   e.preventDefault();
   rubberbandStart(loc.x, loc.y);
};

canvas.onmousemove = function (e) 
{ 
   var loc;
   if (dragging) 
   {
      loc = windowToCanvas(canvas, e.clientX, e.clientY);
      rubberbandStretch(loc.x, loc.y);
   }
}

canvas.onmouseup = function (e) 
{
   rubberbandEnd();
};

// Initialization..............................................

function setImgSrc (theSrc)
{
   image.src = theSrc;
}

image.onload = function () 
{ 
   context.drawImage(image, 0, 0, canvas.width, canvas.height); 
};

resetButton.onclick = function(e) 
{
   context.clearRect(0, 0, canvas.width, canvas.height);
   context.drawImage(image, 0, 0, canvas.width, canvas.height);
   //markerCounter = 0;
};

uploadButton.onclick = function(e) 
{
    //context.clearRect(0, 0, canvas.width, canvas.height);
    //context.drawImage(image, 0, 0, canvas.width, canvas.height);
   var canvas = document.getElementById("canvas");
   var dataURL = canvas.toDataURL("image/png");
	//document.getElementById("saveSignature").src = dataURL; // Is this actually used ???
	
   var dataform = document.getElementById("upload_form");
   dataform.innerHTML += '<input type="hidden" name="image" value="' + dataURL + '"/>';
   
   // LOOP through table of options
   dataform.submit();
};

// DOM string builders
function BuildMarkerInputString( theMarkerIndex, gridsArg='', windowTypeArg='')
{
	// return the synthesized marker options string
}

// Marker data input handlers

function MarkerDataToConsole()
{
	console.log(':::::: MarkerDataToConsole :::::: ');
	if (markerCounter<= 0)
		return;

	var i=0;
	for (i=1; i <= markerCounter; i++)
	{
		console.log(':::::::::::::::: MarkerDataToConsole :::::: i='+i);
		console.log('grids[' + i + ']= ' + grids[i] + ' windowType= ' + windowType[i]);
		console.log('markersDrawn[' + i + ']=  ' + markersDrawn[i]); 
	}
	
}

// JCDeen
function RedrawAll()
{
	console.log(':: RedrawAll console.table(markers) :: markerCounter = ' + markerCounter);
	//if ((markerCounter <= 0) || (markerCounter > 3))
	//	return;
	
	var deviceWidthOverCSSPixels = imageDataCopy.width / canvas.width,
		deviceHeightOverCSSPixels = imageDataCopy.height / canvas.height;
	
	console.table(markers);
	captureCanvasPixels();
	
	var i=0;
	for (i=0; i < markers.length; i++)
	{
		console.log(i);
		obj = JSON.parse(markers[i]);
		rubberbandRectangle.left = obj.left	;
		rubberbandRectangle.top	= obj.top	;
		rubberbandRectangle.width = obj.width;
		rubberbandRectangle.height = obj.height;
		
	   // Put the more transparent image data into the rubberband rectangle
	   context.putImageData(imageDataCopy, 0, 0,
	      (rubberbandRectangle.left + context.lineWidth),
	      (rubberbandRectangle.top + context.lineWidth),
	      (rubberbandRectangle.width - 2*context.lineWidth) * deviceWidthOverCSSPixels,
	      (rubberbandRectangle.height - 2*context.lineWidth)  * deviceHeightOverCSSPixels);
		
	    context.strokeRect(rubberbandRectangle.left + context.lineWidth,
               rubberbandRectangle.top + context.lineWidth,
               rubberbandRectangle.width - 2*context.lineWidth,
               rubberbandRectangle.height - 2*context.lineWidth);
		
		console.table(rubberbandRectangle);
	}
}

// JCDeen
function RemoveMarker(markerIndex)
{
    console.log("markerIndex = " + markerIndex);
	// remove HTML row from markersTable

	var markerRow = document.getElementById("row" + markerIndex);
    markerRow.remove();
    
    markers.splice(markerIndex, 1);
	grids.splice(markerIndex,1);
	windowType.splice(markerIndex,1);
	
    console.log("markers.length = " + markers.length);
    console.table(markers);
}

//Since radio button no need to manage checked/unchecked here.
function CheckGrids(markerIndex, strGrids)
{
	grids[markerIndex] = strGrids;
}

// Since radio button no need to manage checked/unchecked here.
function CheckWindowType(markerIndex, strWindowType)
{
	windowType[markerIndex] = strWindowType;
}

context.strokeStyle = 'navy';
context.font = "normal 25px Arial";
context.fillStyle = 'navy';

context.lineWidth = 2.0;
