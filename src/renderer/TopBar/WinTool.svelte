<script>
  let { ipcRenderer } = require("electron");
  let isMaximized = true;
  let closeWindow = () => {
    if (location.href.includes("/main")) {
      // ipcRenderer.invoke("hideWindow");
      //todo
      ipcRenderer.invoke("closeWindow");
    } else {
      ipcRenderer.invoke("closeWindow");
    }
  };
  let maxmizeMainWin = () => {
    ipcRenderer.invoke("maxmizeWindow");
  };
  let minimizeMainWindow = () => {
    ipcRenderer.invoke("minimizeWindow");
  };
  let unmaximizeMainWindow = () => {
    ipcRenderer.invoke("unmaximizeWindow");
  };
  ipcRenderer.on("windowMaximized", () => {
    isMaximized = true;
  });
  ipcRenderer.on("windowUnmaximized", () => {
    isMaximized = false;
  });
</script>

<div class="winTool">
  <div on:click={minimizeMainWindow}>
    <i class="icon iconminimize" />
  </div>
  {#if isMaximized}
    <div on:click={unmaximizeMainWindow}>
      <i class="icon iconrestore" />
    </div>
  {:else}
    <div on:click={maxmizeMainWin}>
      <i class="icon iconmaximize" />
    </div>
  {/if}
  <div on:click={closeWindow}>
    <i class="icon iconclose" />
  </div>
</div>

<style lang="scss">
  .winTool {
    height: 100%;
    display: flex;
    -webkit-app-region: no-drag;
  }
  .winTool div {
    height: 100%;
    width: 46px;
    text-align: center;
    color: #666666;
    cursor: pointer;
    line-height: 36px;
  }
  .winTool .icon {
    font-size: 12px;
    color: #666666;
  }
  .winTool div:hover {
    background: #e5e5e5;
  }

  .winTool div:last-child:hover {
    background: #f44336;
  }
  .winTool div:last-child:hover i {
    color: #fff !important;
  }
</style>
