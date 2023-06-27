var lavalamp = document.getElementById("lavalamp");

lavalamp.addEventListener("mouseover", function (event) {
  var x = event.clientX - lavalamp.offsetLeft;
  var y = event.clientY - lavalamp.offsetTop;
  var blob = document.createElement("div");
  blob.className = "blob";
  blob.style.left = x + "px";
  blob.style.top = y + "px";
  lavalamp.appendChild(blob);
});

lavalamp.addEventListener("mouseout", function () {
  var blobs = document.getElementsByClassName("blob");
  while (blobs.length > 0) {
    blobs[0].parentNode.removeChild(blobs[0]);
  }
});
