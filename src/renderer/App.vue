<template>
  <div id="app">
    <editor></editor>
    <tabbar></tabbar>
    <div class="main">
      <router-view />
    </div>
    <bottombar></bottombar>
  </div>
</template>

<script>
  import tabbar from "./components/tabbar";
  import bottombar from "./components/bottombar";
  import editor from "./components/editor";
  export default {
    name: 'xiangxuema'
  }
</script>

<style>
  /* CSS */
</style>
