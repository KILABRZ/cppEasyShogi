function canvasOnclick(e) {
	var click_pos = JSON.stringify({x : e.x, y : e.y});
	console.log(click_pos)

	var oReq = new XMLHttpRequest();
	oReq.open("POST", "/", true);
	oReq.send(click_pos);

}

var ctx = null;
var ctx = document.getElementById("shogiboard");
ctx.addEventListener("mousedown", canvasOnclick, false);	


