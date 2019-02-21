<template>
  <div id="app">
    <editor></editor>
    <tabbar></tabbar>
    <router-view />
    <bottombar></bottombar>
  </div>
</template>

<script>
  import tabbar from "./components/tabbar";
  import bottombar from "./components/bottombar";
  import editor from "./components/editor";
  export default {
    components: {
      tabbar,
      bottombar,
      editor
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
        require('electron').remote.getCurrentWindow().on('close', evt => {
          evt.preventDefault()
          self.$root.save(function(){
            electron.remote.app.exit();
          });
        })
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
    }
  }
</script>

<style>
  @import "./assets/app.scss";

  #app {
    height: 100%;
    width: 100%;
    margin: 0px;
    padding: 0px;
    position: absolute;
    font-size: 13px;
    z-index: 9;
    display: flex;
    flex-flow: column;
    background: #f6f6f6;
  }
</style>