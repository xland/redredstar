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
      editor,
    },
    data() {
      return {
      }
    },
    methods: {
    },
    mounted: function () {
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