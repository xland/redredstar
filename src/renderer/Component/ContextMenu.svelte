<script lang="ts">
  import type { ContextMenuModel } from '../../model/ContextMenuModel'

  import { contextMenuStore, contextMenuConfigStore } from '../Store/contextMenuStore'
  let left: number
  let top: number
  let menus: ContextMenuModel[]
  contextMenuStore.subscribe((v) => (menus = v))
  contextMenuConfigStore.subscribe((v) => {
    left = v.clientX
    let totalHeight = menus.length * 30
    if (v.clientY + totalHeight > window.innerHeight) {
      top = v.clientY - totalHeight
    } else {
      top = v.clientY
    }
    let hideContextMenu = () => {
      contextMenuConfigStore.update((config) => {
        config.visible = false
        return config
      })
      document.removeEventListener('mousedown', hideContextMenu)
    }
    document.addEventListener('mousedown', hideContextMenu)
  })
</script>

{#if $contextMenuConfigStore.visible}
  <div style={`left:${left}px;top:${top}px`} class="contextMenuBox">
    {#each menus as menu}
      <div class="menuItem">{menu.title}</div>
    {/each}
  </div>
{/if}

<style lang="scss">
  .contextMenuBox {
    position: absolute;
    box-shadow: 0px 0px 3px #bbb;
    width: 128px;
    background: #fff;
    border-radius: 2px;
    z-index: 2;
    overflow: hidden;
  }
  .menuItem {
    height: 30px;
    line-height: 30px;
    padding: 0px 8px;
  }
  .menuItem:hover {
    background: rgb(205, 255, 216);
  }
</style>
