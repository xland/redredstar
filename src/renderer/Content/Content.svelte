<script lang="ts">
  import CategoryTool from './Category/CategoryTool.svelte'
  import CategoryTree from './Category/CategoryTree.svelte'
  import TitleTool from './Title/TitleTool.svelte'
  import TitleList from './Title/TitleList.svelte'
  import Editor from './Editor.svelte'
  import { globalObjs } from '../Store/globalObjs'
</script>

<div class="content">
  <div class="category">
    <CategoryTool />
    <CategoryTree />
  </div>
  <div class="titleList">
    <TitleTool />
    <TitleList />
  </div>
  <div on:click={() => globalObjs.editor.focus()} class="articleContent">
    <Editor />
  </div>
</div>

<style lang="scss">
  .category {
    width: 180px;
    display: flex;
    flex-direction: column;
  }
  .content {
    display: flex;
    height: calc(100% - 68px);
    overflow: hidden;
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
</style>
