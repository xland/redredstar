<template>
  <div id="app">
    <div class="viewAndMenuContainer">
      <menubar></menubar>
      <router-view />
    </div>
    <bottombar></bottombar>
  </div>
</template>

<script>
  import bottombar from "./components/bottombar";
  import menubar from "./components/menubar";
  const path = require('path');
  const fs = require('fs-extra')
  const {
    ipcRenderer,
    remote
  } = require('electron');
  export default {
    components: {
      bottombar,
      menubar,
    },
    data() {
      return {}
    },
    methods: {},
    mounted() {
      this.db("settings").select("*").then(rows => this.$root.init(rows[0]));
      ipcRenderer.on('articlePublishRenderer', (e, message) => {
        this.$root.articlePublushCb(message);
      })
      ipcRenderer.on('imgUploadRenderer', (e, message) => {
        this.$root.imgUploadCb(message);
      });
      ipcRenderer.on('articleFromWebApp', (e, message) => {
        let article = {
          title: message.title,
          created_at: new Date(),
          updated_at: new Date(),
          editor_type: "html",
        };
        this.db("articles").insert(article).then(rows => {
          article.id = rows[0];
          let aPath = path.join(remote.app.getPath('userData'), "/xxm/" + article.id);
          fs.mkdirSync(aPath);
          fs.writeFileSync(path.join(aPath, "/a.data"), message.content, this.$root.rwOption);
          this.bus.$emit('articleCount');
          this.bus.$emit('articleFromWebApp');
        })
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

  #ckEditorContainer {
    position: absolute;
    z-index: 10;
    left: 8px;
    right: 8px;
    top: 78px;
    bottom: 72px;
  }

  #ckEditor {
    width: 100%;
    height: 100%;
    margin: 0px;
    padding: 0px;
    border: none;
    outline: none;
    resize: none;
  }

  .viewAndMenuContainer {
    flex: 1;
    overflow: hidden;
    display: flex;
    margin-top: 8px;
  }
</style>