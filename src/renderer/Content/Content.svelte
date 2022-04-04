<script lang="ts">
  import CategoryTool from './Category/CategoryTool.svelte'
  import CategoryTree from './Category/CategoryTree.svelte'
  import TitleTool from './Title/TitleTool.svelte'
  import TitleList from './Title/TitleList.svelte'
  import Editor from './Editor.svelte'
  import { globalObjs } from '../Store/globalObjs'
  import { onMount } from 'svelte'
  import Spliter from './Spliter.svelte'
  onMount(() => {
    let categoryDom = document.querySelector('.category')
    categoryDom.setAttribute('style', 'width:200px')
    let titleListDom = document.querySelector('.titleList')
    titleListDom.setAttribute('style', 'width:320px')
  })
</script>

<div class="content">
  <div class="category">
    <CategoryTool />
    <CategoryTree />
    <Spliter />
  </div>
  <div class="titleList">
    <TitleTool />
    <TitleList />
    <Spliter />
  </div>
  <div on:click={() => globalObjs.editor.focus()} class="articleContent">
    <Editor />
  </div>
</div>

<style lang="scss">
  .content {
    display: flex;
    height: calc(100% - 68px);
    overflow: hidden;
  }
  .category {
    display: flex;
    flex-direction: column;
    flex-shrink: 0;
    position: relative;
  }
  .titleList {
    display: flex;
    flex-direction: column;
    flex-shrink: 0;
    position: relative;
  }
  .articleContent {
    flex: 1;
    cursor: text;
  }
</style>
