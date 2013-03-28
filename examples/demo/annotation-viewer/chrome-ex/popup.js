var viewer = {
    requestURL: "https://people.csail.mit.edu/uchida/work/semweb/demo/annotation-viewer/extract.cgi?url=",
    
    show: function(url) {
      var req = new XMLHttpRequest();
      req.open("GET", viewer.requestURL + url, true);
      document.getElementById("container").innerHTML = '<img src="treeview/images/ajax-loader.gif">';
      req.onreadystatechange = function() {
        if (req.readyState == 4) {
          var content = JSON.parse(this.responseText);
          viewer.showTree(content);
        }
      };
      req.send(null);
    },
    showTree: function(json) {
      var html_str="";
      var traverse = function(key, jsonObj) {
        if(jsonObj!= null && typeof(jsonObj) == "object") {     
          $.each(jsonObj, function(k,v) {
            if(typeof(k) == "string") {
              var clazz = "";
              if(k.substring(0, "__ITEM__:".length) == "__ITEM__:") {
                clazz = "folder";
                k = k.substring("__ITEM__:".length);
              } else if(k.substring(0, "__ATTR__:".length) == "__ATTR__:") {
                clazz = "file";
                k = k.substring("__ATTR__:".length);
              }
              html_str += '<li class="opened"><span class="' + clazz + '">' + k + '</span><ul>';
              traverse(k, v);
              html_str += '</ul></li>';
            } else {
              traverse(k, v);
            }
          });       
        }
        else {
          // jsonOb is a number or string
          if(jsonObj!= null && typeof(jsonObj) == "string") {
            var v_str = jsonObj;
            if(v_str.substring(0, "__URL__:".length) == "__URL__:") {
              v_str = v_str.substring("__URL__:".length);
              v_str = '<a href="' + v_str + '">' + v_str + '</a>';
            }
            html_str += '<li><span>' + v_str + '</span></li>';
          } else {
            html_str += '<li><span>' + jsonObj + '</span></li>';
          }
        }
        return html_str;
      };
      var show_json_as_tree=function(jsonObj){
        var str = '<ul class="filetree">' + traverse("",jsonObj) + '</ul>';
        $('div#container').empty().html(str).treeview({
          animated: "normal",
          control: "#treecontrol",
          persist: "location"
        }); 
      };
      show_json_as_tree(json);
    }
};
document.addEventListener('DOMContentLoaded', function () {
  chrome.tabs.query({
    "active": true,
    "windowId": chrome.windows.WINDOW_ID_CURRENT
  }, function(tabs) {
    var tab = tabs.length === 0 ? tabs : tabs[0];
    viewer.show(tab.url);
  });
  document.getElementById("container").style.visibility = "visible";
});
