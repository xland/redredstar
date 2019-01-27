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
            window.nw.App.quit();
            //window.nw.Window.get().close(true);
          });
        });
      },
      autoSave(){
        var self = this
        setInterval(function(){
          self.$root.save();
        },this.$root.u.autoSaveIntervalSeconds*1000);
      }
    },
    mounted: function () {
      this.hookWinClose();
      this.autoSave();
      //nw.Window.get().showDevTools(); //todo:     
    }
  };
</script>
<style>

</style>