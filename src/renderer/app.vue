<template>
  <div id="app">
    <div class="viewAndMenuContainer">
      <menubar></menubar>
      <router-view :key="$route.fullPath" />
    </div>
    <bottombar></bottombar>
  </div>
</template>

<script>
const { remote } = require("electron");
import bottombar from "./components/bottombar";
import menubar from "./components/menubar";
export default {
  components: { bottombar, menubar },
  data() {
    return {};
  },
  methods: {
    async init(setting) {
      this.$root.init(setting);
      this.$root.hookMsgFromMainProcess();
    },
    async onResize(setting) {
      let win = remote.getCurrentWindow();
      if (setting.win_size != "maximize") {
        let arr = setting.win_size.split("*");
        win.setSize(parseInt(arr[0]), parseInt(arr[1]));
      } else {
        win.maximize();
      }
      win.on("maximize", async _ => {
        await this.db("settings").update({ win_size: "maximize" });
      });
      win.on("resize", async _ => {
        let size = win.getSize();
        await this.db("settings").update({ win_size: `${size[0]}*${size[1]}` });
      });
    }
  },
  async mounted() {
    let setting = await this.db("settings")
      .select("*")
      .first();
    this.init(setting);
    this.onResize(setting);
  }
};
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