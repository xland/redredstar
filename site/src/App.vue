<template>
  <div id="app">
    <div class="top"></div>
    <!-- <div id="nav">
      <router-link to="/">Home</router-link> |
      <router-link to="/about">About</router-link>
    </div>
    <router-view/>-->
  </div>
</template>

<style>
html,
body {
  margin: 0px;
  padding: 0px;
  overflow: hidden;
}
.top {
  height: 8000px;
  width: 8000px;
  background: #f3f4f6;
  border-bottom-left-radius:4000px;
  border-bottom-right-radius: 4000px;
  position: absolute;
  top: -7420px;
  left: calc(50% - 4000px);
}
#app {
  font-family: "Avenir", Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
}
#nav {
  padding: 30px;
}

#nav a {
  font-weight: bold;
  color: #2c3e50;
}

#nav a.router-link-exact-active {
  color: #42b983;
}
</style>
