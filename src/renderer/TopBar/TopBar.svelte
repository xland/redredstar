<script lang="ts">
  import WinTool from "./WinTool.svelte";
</script>

<div class="topBar">
  <div class="winTitle">标题</div>
  <WinTool />
</div>

<style lang="scss">
  .topBar {
    display: flex;
    height: 38px;
    line-height: 38px;
    background: #eee;
    width: 100%;
  }
  .winTitle {
    flex: 1;
  }
</style>
