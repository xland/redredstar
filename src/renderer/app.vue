<template>
  <div id="app">
    <div class="viewAndMenuContainer">
      <menubar></menubar>
      <router-view :key="$route.fullPath" />
    </div>
    <bottombar></bottombar>
    <!-- <webview disablewebsecurity preload="file:///project/xiangxuema/src/renderer/test.js" style="height: 100%"
      src="https://item.taobao.com/item.htm?id=548238354539">
    </webview> -->
  </div>
</template>

<script>
  import bottombar from "./components/bottombar";
  import menubar from "./components/menubar";
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
      // const webview = document.querySelector('webview')
      // webview.addEventListener('dom-ready', () => {
      //   webview.openDevTools()
      // })
      // return;

      this.db("settings").select("*").then(rows => this.$root.init(rows[0]));
      this.$root.hookMsgFromMainProcess();
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