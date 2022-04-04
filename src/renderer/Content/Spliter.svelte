<script lang="ts">
  import { onMount } from 'svelte'
  let spliter: HTMLElement
  let spliterDraging = (e: MouseEvent) => {
    let parent = spliter.parentElement
    if (parent.classList.contains('category')) {
      parent.setAttribute('style', `width:${e.clientX}px`)
    } else {
      parent.setAttribute('style', `width:${e.clientX - parent.offsetLeft}px`)
    }
  }
  let spliterDragEnd = () => {
    document.removeEventListener('mousemove', spliterDraging)
    document.removeEventListener('mouseup', spliterDragEnd)
  }
  let spliterMousedown = (e: MouseEvent) => {
    document.addEventListener('mousemove', spliterDraging)
    document.addEventListener('mouseup', spliterDragEnd)
  }
</script>

<div on:mousedown={spliterMousedown} class="spliter" bind:this={spliter} />

<style lang="scss">
  .spliter {
    position: absolute;
    z-index: 1;
    width: 6px;
    height: 100%;
    right: -3px;
  }
  .spliter:hover {
    background: rgb(225, 228, 232);
    cursor: w-resize;
  }
</style>
