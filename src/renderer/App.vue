<template>
  <div id="app" v-if="dbReady">
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
  const path = require('path');
  const fs = require('fs-extra')
  const {
    ipcRenderer,
    remote
  } = require('electron');
  export default {
    components: {
      tabbar,
      bottombar,
      editor
    },
    data() {
      return {
        dbReady: false,
      }
    },
    methods: {
      hookSaveArticle() {
        var self = this;
        remote.getCurrentWindow().on('close', (event) => {
          event.preventDefault();
          self.bus.$emit('saveContent');
          setTimeout(function () {
            remote.app.quit();
          }, 58);
        })
      },
      checkDb() {
        let bakDir = path.join(remote.app.getPath('userData'), "/xxm_bak");
        if (fs.existsSync(bakDir)) {
          this.dbReady = true;
        } else {
          var self = this;
          window.dbReady = function () {
            self.dbReady = true;
          }
        }
      }
    },
    mounted: function () {
      this.hookSaveArticle();
      this.checkDb();
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