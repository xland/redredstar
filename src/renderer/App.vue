<template>
  <div id="app">
    <tabbar></tabbar>
    <router-view />
    <bottombar></bottombar>
  </div>
</template>

<script>
  import tabbar from "./components/tabbar";
  import bottombar from "./components/bottombar";

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
    },
    data() {
      return {}
    },
    methods: {},
    mounted() {
      this.db("settings").select("*").then(rows => {
        this.$root.tickStep = rows[0].autosave_interval * 1000;
        this.$root.imgHight = rows[0].img_h;
        this.$root.imgWidth = rows[0].img_w;
        this.$root.editorType = rows[0].editor_type;
      });
      ipcRenderer.on('articlePublishRenderer', (e, message) => {
        this.$root.articlePublushCb(message);
      })
      ipcRenderer.on('imgUploadRenderer', (e, message) => {
        this.$root.imgUploadCb(message);
      })
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