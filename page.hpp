#ifndef PAGE_HPP_
#define PAGE_HPP_
#include <Arduino.h>

static const inline String page =
R"xxxx(

<!DOCTYPE html>
  <html lang="fr">
  <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    
    <style>
  .collapsible {
    background-color: #777;
    color: white;
    cursor: pointer;
    padding: 18px;
    width: 100%;
    border: none;
    text-align: left;
    outline: none;
    font-size: 15px;
  }

  .active, .collapsible:hover {
    background-color: #555;
  }

  .content {
    padding: 0 18px;
    display: none;
    overflow: hidden;
    background-color: #f1f1f1;
  }
    </style>

       
  <title>Lampe</title>
  </head>

  <body>

    <h1 class="collapsible" id="color1">Une couleur</h1>
    <div class="content">
    <form action="#" method="post">
    <table>
    <tr>
    <th><label for="c1_0">Couleur</label></th>
    <td><input type="color" id="c1_0" name="c0" value="$c0$" ></td>
    </tr>
    </table>
    <input type="submit" value="OK">
    </form>
    </div>

    <h1 class="collapsible" id="color2">Deux couleurs</h1>
    <div class="content">
    <form action="#" method="post">
    <table>
    <tr>
    <th><label for="c2_0">Couleur 1</label></th>
    <td><input type="color" id="c2_0" name="c0" value="$c0$" ></td>
    </tr>
    <tr>
    <th><label for="c2_1">Couleur 2</label></th>
    <td><input type="color" id="c2_1" name="c1" value="$c1$" ></td>
    </tr>
  
  <tr>
    <th><label for="c2_duration">Duration (s)</label></th>
    <td><input type="number" step="0.1" min="0" max="3600" id="c2_duration" name="duration" value="$duration$" ></td>
    </tr>
  
    </table>
    <input type="submit" value="OK">
    </form>
    </div>

    <h1 class="collapsible" id="color3">Trois couleurs</h1>
    <div class="content">
    <form action="#" method="post">
    <table>
    <tr>
    <th><label for="c3_0">Couleur 1</label></th>
    <td><input type="color" id="c3_0" name="c0"  value="$c0$" ></td>
    </tr>
    <tr>
    <th><label for="c3_1">Couleur 2</label></th>
    <td><input type="color" id="c3_1" name="c1"  value="$c1$" ></td>
    </tr>
    <tr>
    <th><label for="c3_2">Couleur 3</label></th>
    <td><input type="color" id="c3_2" name="c2"  value="$c2$" ></td>
    </tr>
    <tr>
    <th><label for="c3_duration">Duration (s)</label></th>
    <td><input type="number" step="0.1" min="0" max="3600" id="c3_duration" name="duration" value="$duration$" ></td>
    </tr>
    </table>
    <input type="submit" value="OK">
    </form>
    </div>

    <h1 class="collapsible" id="color4">Quatre couleurs</h1>
    <div class="content">
    <form action="#" method="post">
    <table>
    <tr>
    <th><label for="c4_0">Couleur 1</label></th>
    <td><input type="color" id="c4_0" name="c0"  value="$c0$" ></td>
    </tr>
    <tr>
    <th><label for="c4_1">Couleur 2</label></th>
    <td><input type="color" id="c4_1" name="c1"  value="$c1$" ></td>
    </tr>
    <tr>
    <th><label for="c4_2">Couleur 3</label></th>
    <td><input type="color" id="c4_2" name="c2"  value="$c2$" ></td>
    </tr>
    <tr>
    <th><label for="c4_3">Couleur 4</label></th>
    <td><input type="color" id="c4_3" name="c3" value="$c3$" ></td>
    </tr>
  <tr>
    <th><label for="c4_duration">Duration (s)</label></th>
    <td><input type="number" step="0.1" min="0" max="3600" id="c4_duration" name="duration" value="$duration$" ></td>
    </tr>
    </table>
    <input type="submit" value="OK">
    </form>
    </div>
  
  
    <h1 class="collapsible">Wifi</h1>
    <div class="content">
    
  <b>Status</b>
  <br>
    <table>
    <tr><td>Status</td><td>$wifi_mode$</td></tr>
    <tr><td>MAC</td><td>$wifi_mac$</td></tr>
    <tr><td>IP</td><td>$wifi_ip$</td></tr>
    <tr><td>SSID</td><td>$wifi_ssid$</td></tr>
    <tr><td>Stations connectées</td><td>$wifi_stationNum$</td></tr>
    <tr><td title="Tente de se connecter au wifi en mode station une fois le délais écoulé">AP timeout</td><td>$wifi_timeout$</td></tr>      
    </table>
  <br>
    
    
    <b>Connection</b>
  <br>
    <form action="#" method="post">
    <table>
    <tr><td><label for="wifi_ssid">SSID</label></td><td><input type="text" id="wifi_ssid" name="wifi_ssid" value="$wifi_st_ssid$" /></td></tr>
    <tr><td><label for="wifi_pass">Pass</label></td><td><input type="text" id="wifi_pass" name="wifi_pass" value="" /></td></tr>
    </table>
    <br>
    <input type="submit" value="OK">
    </form>
    </div>

    

  <script>
  var coll = document.getElementsByClassName("collapsible");
  var i;

  for (i = 0; i < coll.length; i++) {
    coll[i].addEventListener("click", function() {
      //hide others
      var hide_list = document.getElementsByClassName("collapsible");
    for (i = 0; i < hide_list.length; i++) {
        x=hide_list[i];
      x.classList.remove("active");
      x.nextElementSibling.style.display = "none";
    };
  
      //show this
      this.classList.add("active");
      this.nextElementSibling.style.display = "block";
      }
  )};
  
  
  
  document.getElementById("color$show_me$").click();
  </script>
 

  </body>
</html>


)xxxx";




#endif
