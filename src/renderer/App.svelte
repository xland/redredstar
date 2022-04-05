<script lang="ts">
  import TopBar from './TopBar/TopBar.svelte'
  import { ipcRenderer } from 'electron'
  import { onMount } from 'svelte'
  import Content from './Content/Content.svelte'
  import StatusBar from './StatusBar/StatusBar.svelte'
  import ContextMenu from './Component/ContextMenu.svelte'
  onMount(() => {
    ipcRenderer.invoke('maxmizeWindow')
  })
</script>

<TopBar />
<Content />
<StatusBar />
<ContextMenu />

<style lang="scss">
</style>
