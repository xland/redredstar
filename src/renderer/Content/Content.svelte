<script lang="ts">
  import CategoryTool from './Category/CategoryTool.svelte'
  import CategoryTree from './Category/CategoryTree.svelte'
  import TitleTool from './Title/TitleTool.svelte'
  import TitleList from './Title/TitleList.svelte'
  import Editor from './Editor.svelte'
  import { globalObjs } from '../Store/globalObjs'
  import { onMount } from 'svelte'
  let dragingDom: HTMLElement
  let setCategoryWidth = (width = 180) => {
    let categoryDom = document.querySelector('.category')
    let titleListDom = document.querySelector('.titleList') as HTMLElement
    let categorySplitter = document.getElementById('categorySplitter')
    let titleSplitter = document.getElementById('titleSplitter')
    categoryDom.setAttribute('style', `width:${width}px`)
    categorySplitter.setAttribute('style', `left:${width - 3}px`)
    titleSplitter.setAttribute('style', `left:${titleListDom.clientWidth + width - 3}px`)
  }
  let setTitleListWidth = (totalWidth = 500) => {
    let titleListDom = document.querySelector('.titleList') as HTMLElement
    let titleSplitter = document.getElementById('titleSplitter')
    titleListDom.setAttribute('style', `width:${totalWidth - titleListDom.offsetLeft}px`)
    titleSplitter.setAttribute('style', `left:${totalWidth - 3}px`)
  }
  let spliterDraging = (e: MouseEvent) => {
    if (dragingDom.id === 'categorySplitter') {
      setCategoryWidth(e.clientX)
    } else {
      setTitleListWidth(e.clientX)
    }
  }
  let spliterDragEnd = () => {
    document.removeEventListener('mousemove', spliterDraging)
    document.removeEventListener('mouseup', spliterDragEnd)
    dragingDom = null
  }
  let spliterMousedown = (e: MouseEvent) => {
    document.addEventListener('mousemove', spliterDraging)
    document.addEventListener('mouseup', spliterDragEnd)
    dragingDom = e.target as HTMLElement
  }
  onMount(() => {
    setCategoryWidth()
    setTitleListWidth()
  })
</script>

<div class="content">
  <div class="category">
    <CategoryTool />
    <CategoryTree />
  </div>
  <div on:mousedown={spliterMousedown} id="categorySplitter" class="spliter" />
  <div class="titleList">
    <TitleTool />
    <TitleList />
  </div>
  <div on:mousedown={spliterMousedown} id="titleSplitter" class="spliter" />
  <div on:click={() => globalObjs.editor.focus()} class="articleContent">
    <Editor />
  </div>
</div>

<style lang="scss">
  .content {
    display: flex;
    height: calc(100% - 68px);
    overflow: hidden;
    position: relative;
  }
  .category {
    width: 180px;
    display: flex;
    flex-direction: column;
  }
  .titleList {
    display: flex;
    flex-direction: column;
    width: 320px;
  }
  .articleContent {
    flex: 1;
    cursor: text;
  }
  .spliter {
    position: absolute;
    z-index: 1;
    width: 6px;
    height: 100%;
  }
  .spliter:hover {
    background: rgb(225, 228, 232);
    cursor: w-resize;
  }
</style>
