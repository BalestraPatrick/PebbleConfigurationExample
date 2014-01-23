PebbleConfigurationExample
==========================

A simple Pebble app and a website to demonstrate how to create a configuration page for your app.
The Settings.html file goes on your webserver and it's loaded in the webView of the Pebble app when the user presses "Settings". This should go on your webserver so you can easily modify it.
The value are stored in the localStorage of the website in the Pebble app so the values on the watch and on the phone are always the same. A JSON file it's then built from this values and passed to the watch.
The watch will read the value and save to its Persistent Storage and show it in a TextLayer.

If you have any questions, contact me on Twitter: <a href="http://www.twitter.com/BalestraPatrick">@BalestraPatrick</a>

Feel free to improve the code :)
