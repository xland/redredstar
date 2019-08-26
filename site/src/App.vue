<template>
  <div id="app">
    <router-view></router-view>
  </div>
</template>
<script>
import topMenu from "./components/topMenu";
import titleContent from "./components/titleContent";
export default {
  components: {
    topMenu,
    titleContent
  },
  data() {
    return {};
  },
  mounted() {},
  methods: {}
};
</script>
<style>
html,
body {
  margin: 0px;
  padding: 0px;
  overflow-x: hidden;
}
::-webkit-scrollbar {
  width: 6px;
  height: 6px;
  background-color: #eee;
}

::-webkit-scrollbar-track {
  border-radius: 0px;
  background-color: #eee;
}

::-webkit-scrollbar-thumb {
  border-radius: 0px;
  background-color: #ccc;
}
#app {
  font-family: "Avenir", Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
}
</style>
