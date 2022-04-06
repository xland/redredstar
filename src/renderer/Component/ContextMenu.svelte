<script lang="ts">
  import { onMount } from 'svelte'
  import { eventer } from '../../common/eventer'
  let left: number
  let top: number
  let visible = false
  let menus = []
  eventer.on('showContextMenu', (menuArr, position) => {
    let totalHeight = menuArr.length * 30
    if (position.y + totalHeight > window.innerHeight) {
      top = position.y - totalHeight
    } else {
      top = position.y
    }
    if (position.x + 158 > window.innerWidth) {
      left = position.x - 158
    } else {
      left = position.x
    }
    menus = menuArr
    visible = true
  })
  onMount(() => {
    document.addEventListener('mousedown', () => {
      visible = false
    })
  })
</script>

{#if visible}
  <div style={`left:${left}px;top:${top}px`} class="contextMenuBox">
    {#each menus as menu}
      <div on:mousedown={menu.onClick} class="menuItem">
        {menu.title}
      </div>
    {/each}
  </div>
{/if}

<style lang="scss">
  .contextMenuBox {
    position: absolute;
    box-shadow: 0px 0px 3px #bbb;
    width: 158px;
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
