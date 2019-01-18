<template>
  <div id="app">
    <editor></editor>
    <titlebar></titlebar>
    <tabbar></tabbar>
    <div class="main">
      <router-view />
    </div>
    <bottombar></bottombar>
  </div>
</template>
<script>
  var fs = nw.require('fs');
  var path = nw.require('path');
  import swal from 'sweetalert';
  import titlebar from "./components/titlebar";
  import tabbar from "./components/tabbar";
  import bottombar from "./components/bottombar";
  import editor from "./components/editor";
  export default {
    components: {
      titlebar,
      tabbar,
      bottombar,
      editor
    },
    data() {
      return {};
    },
    watch: {
      "$root.u": {
        handler: function (val, oldVal) {
          this.$root.needSave.u = true;
        },
        deep: true
      },
      "$root.t": {
        handler: function (val, oldVal) {
          this.$root.needSave.t = true;
        },
        deep: true
      },
      "$root.a": {
        handler: function (val, oldVal) {
          this.$root.needSave.a = true;
        },
        deep: true
      },
    },
    methods: {
      hookWinClose() {
        var self = this;
        window.nw.Window.get().on('close', function () {
          self.$root.save(function(){
            window.nw.Window.get().close(true);
          });
        });
      },
      autoSave(){
        var self = this
        setInterval(function(){
          self.$root.save();
        },this.$root.u.autoSaveIntervalSeconds*1000);
      },
      checkVersion() {
        //todo:用真正的版本号检测是否该升级
        const request = httpUtil.get(
          'https://gitee.com/xland/cnblogs/releases',
          function (responseText) {
            var doc = new DOMParser().parseFromString(responseText, "text/html");
            var v = doc.getElementsByClassName('tag-name')[0].innerText.trim();
            var arr = v.split('.').map((v) => {
              return parseInt(v)
            });
            var curArr = window.nw.App.manifest.version.split('.').map((v) => {
              return parseInt(v);
            });
            var flag = false;
            if (arr[0] > curArr[0]) {
              flag = true;
            } else if (arr[0] == curArr[0]) {
              if (arr[1] > curArr[1]) {
                flag = true;
              } else if (arr[1] == curArr[1]) {
                if (arr[2] > curArr[2]) {
                  flag = true;
                }
              }
            }
            if (flag) {
              swal({
                icon: "info",
                text: "有新版本可供升级",
                buttons: [
                  true, "去升级！"
                ]
              }).then((value) => {
                if (!value) return;
                window.nw.Shell.openExternal('https://gitee.com/xland/cnblogs/releases');
              })
            }
          })
      }
    },
    mounted: function () {
      this.checkVersion();
      this.hookWinClose();
      this.autoSave();
      //nw.Window.get().showDevTools(); //todo:     
    }
  };
</script>
<style>

</style>