<script lang="ts">
  import WinTool from './WinTool.svelte'
</script>

<div class="topBar">
  <div class="winTitle">RedRedStar 个人知识管理工具</div>
  <WinTool />
</div>

<style lang="scss">
  .topBar {
    display: flex;
    height: 38px;
    line-height: 38px;
    background: #eee;
    width: 100%;
    -webkit-app-region: drag;
  }
  .winTitle {
    flex: 1;
    padding-left: 12px;
  }
</style>
