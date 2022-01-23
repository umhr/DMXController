#include "Arduino.h"
#include "DMX.h"

DMX::DMX(void){
  
}

String DMX::getHTML(void){
  //return "ageoiuge";
  /**/
  String result = "<!DOCTYPE html><html lang='ja'><head><meta charset='utf-8'><title>DMX</title><style type='text/css'></style></head>";
  result += "<body>";
  result += "<div><a href='../'>../</a></div>";
  result += "<div id='range'></div>\r\n";
  result += "<script>\r\n";
  result += "document.getElementById('range').innerHTML = getRenge();\r\n";
  
  result += "function getRenge(){\r\n";
  result += "var list = ['Pan','Pan fine', 'Tilt', 'Tilt fine', 'Speed', 'Dimming', 'Red Dimming', 'Green Dimming', 'Blue Dimming', 'White Dimming', 'Color Choose', 'Color Speed', 'Auto', 'Reset'];\r\n";
  result += "var result = '';\r\n";
  result += "for(var i = 0; i < list.length; i++){\r\n";
  result += "result += '<div><label>' + list[i] + '<label>';\r\n";
  result += "result += '<input type=\\'range\\' name=\\'' + (i+1) + '\\' min=\\'0\\' value=\\'0\\' max=\\'255\\' onchange=\\'onChange(this)\\'>';\r\n";
  result += "result += '</div>';\r\n";
  result += "}\r\n";
  result += "return result;\r\n";
  result += "}\r\n";
  
  result += "function onChange(self){\r\n";
  result += "var xhr = new XMLHttpRequest;\r\n";
  result += "xhr.open('GET', './?c=' + self.name + '&w=' + self.value);\r\n";
  result += "xhr.send(null);\r\n";
  result += "}\r\n";
  result += "</script>\r\n";
  result += "</body></html>";
  return result;
  /**/
}
