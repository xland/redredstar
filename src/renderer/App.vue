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
  const { ipcRenderer,remote } = require('electron');
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
      autoSave() {
        var self = this
        setInterval(function () {
          self.$root.save();
        }, this.$root.u.autoSaveIntervalSeconds * 1000);
      },
      hookSaveArticle() {
        var self = this;
        ipcRenderer.on('saveArticleRenderer', (e, message) => {
          self.$root.save();
          ipcRenderer.send('appQuit', {});
        });
      },
    },
    mounted: function () {
      this.hookSaveArticle();
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