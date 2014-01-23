
Pebble.addEventListener("ready", function(e) {
    console.log("Ready");
});

Pebble.addEventListener("showConfiguration", function (e) {
    /* CHANGE THIS LINK TO YOUR WEB SERVER */
    Pebble.openURL("http://patrickbalestra.com/settings.html");
});

Pebble.addEventListener("webviewclosed", function(e) {
    console.log(e.response);
    var responseFromWebView = decodeURIComponent(e.response);
    var settings = JSON.parse(responseFromWebView);
    Pebble.sendAppMessage(settings);
});

