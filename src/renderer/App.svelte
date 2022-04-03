<script lang="ts">
  import TopBar from './TopBar/TopBar.svelte'
  import { ipcRenderer } from 'electron'
  import { onMount } from 'svelte'
  import Content from './Content/Content.svelte'
  import StatusBar from './StatusBar/StatusBar.svelte'
  onMount(() => {
    ipcRenderer.invoke('maxmizeWindow')
  })
</script>

<TopBar />
<Content />
<StatusBar />

<style lang="scss">
</style>
