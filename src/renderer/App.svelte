<script lang="ts">
  import TopBar from './TopBar/TopBar.svelte'
  import { ipcRenderer } from 'electron'
  import { onMount } from 'svelte'
  import Content from './Content/Content.svelte'
  onMount(() => {
    ipcRenderer.invoke('showWindow')
  })
</script>

<TopBar />
<Content />

<style lang="scss">
</style>
