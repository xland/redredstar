<template>
  <div id="app">
    <div class="top"></div>
    <div class="header">
      <div class="logo">
        <div>
          <img src="@/assets/logo.png" />
        </div>
        <div class="logoTitle">想学吗</div>
      </div>
      <div class="menu">
        <div>功能介绍</div>
        <div>免责声明</div>
        <div>二开说明</div>
        <div>联系方式</div>
      </div>
    </div>
    <div class="title">想学吗——个人知识管理工具</div>
    <div class="subTitle">想学吗可以轻松的构建个人知识架构，包含知识、脑图、火花等几大模块</div>
    <div class="download">
      <div :class="[!isMac?'cur':'']">立即下载Windows版</div>
      <div :class="[isMac?'cur':'']">立即下载Mac版</div>
    </div>
    <!-- <div id="nav">
      <router-link to="/">Home</router-link> |
      <router-link to="/about">About</router-link>
    </div>
    <router-view/>-->
  </div>
</template>
<script>
export default {
  data() {
     return {
       isMac: navigator.userAgent.indexOf("Mac OS X") > 0
     }
  },
  mounted() {},
  methods: {
  }
};
</script>

<style>
html,
body {
  margin: 0px;
  padding: 0px;
  overflow-x: hidden;
}
.top {
  height: 8000px;
  width: 8000px;
  background: #f3f4f6;
  border-bottom-left-radius: 4000px;
  border-bottom-right-radius: 4000px;
  position: absolute;
  top: -7420px;
  left: calc(50% - 4000px);
  z-index: -1;
}
.header {
  height: 44px;
  line-height: 44px;
  position: fixed;
  display: flex;
  padding-left: 16%;
  padding-right: 16%;
  left: 0px;
  right: 0px;
  top: 0px;
  padding-top: 12px;
  padding-bottom: 12px;
  line-height: 44px;
}
.logo {
  width: 260px;
}
.logoTitle {
  color: #666;
  font-size: 18px;
  font-weight: bolder;
  line-height: 44px;
}
.logo div {
  float: left;
}
.logo div img{
  height: 36px;
  margin-top: 4px;
  margin-right: 8px;
}
.menu {
  flex: 1;
}
.menu div {
  float: right;
  padding-left: 16px;
  padding-right: 16px;
  color: #555;
}
.menu div:hover {
  color: #0084ff;
  cursor: pointer;
}
.title {
  font-size: 36px;
  font-weight: bolder;
  color: #555;
  margin-top: 68px;
  padding-top: 80px;
}
.subTitle {
  padding-top: 36px;
  line-height: 28px;
  font-size: 18px;
  color: #666;
}
.download {
  margin-top: 38px;
}
.download div {
  display: inline-block;
  height: 56px;
  line-height: 56px;
  width: 238px;
  text-align: center;
  border-radius: 6px;
  background: #fff;
  margin-left: 16px;
  margin-right: 16px;
  border: 1px solid #ccc;
  cursor: pointer;
}
.download div:hover{
  background: #0064ef;
  color: #fff;
  border: 1px solid #0064ef;
}
.download .cur {
  background: #0084ff;
  color: #fff;
  border: 1px solid #0084ff;
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
