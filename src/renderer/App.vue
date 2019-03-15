<template>
  <div id="app">
    <tabbar></tabbar>
    <router-view />
    <bottombar></bottombar>
    <div v-if="false" v-show="$route.params.id && !$root.curArticleMd" id="editorU"></div>
    <div id="ckEditorContainer">
      <textarea id="ckEditor"></textarea>
    </div>
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
      ipcRenderer.on('updateRenderer', (e, message) => {
        swal({
          icon: "info",
          text: "新版本已经为您准备好啦！\n现在升级？还是下次启动应用时再升级？",
          closeOnClickOutside: false,
          closeOnEsc: false,
          buttons: [
            "下次升级", "现在升级"
          ]
        }).then((value) => {
          if (!value) return;
          ipcRenderer.send('updateMain');
        });
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

  #editorU,#ckEditorContainer {
    position: absolute;
    z-index: 10;
    left: 8px;
    right: 8px;
    top: 78px;
    bottom: 72px;
  }
  #ckEditor{
    width: 100%;height: 100%;
    margin: 0px;
    padding: 0px;
    border:none;
    outline: none;
    resize: none;
  }
</style>