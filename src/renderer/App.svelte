<script lang="ts">
  import TopBar from "./TopBar/TopBar.svelte";
  import { ipcRenderer } from "electron";
  import { onMount } from "svelte";
  import "./icon/iconfont.css";
  export let name: string;
  onMount(() => {
    ipcRenderer.invoke("maxmizeWindow");
  });
</script>

<TopBar />

<style lang="scss">
</style>
