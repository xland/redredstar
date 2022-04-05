<script lang="ts">
  import { onMount } from 'svelte'
  import type { CategoryModel } from '../../../model/CategoryModel'
  import { categoryStore } from '../../Store/categoryStore'
  export let category: CategoryModel
  let eachCategory = (categorys: CategoryModel[]) => {
    for (let element of categorys) {
      if (element.isSelected && element.id != category.id) element.isSelected = false
      if (!element.subCategory || element.subCategory.length < 1) continue
      eachCategory(element.subCategory)
    }
  }
  let categoryClick = () => {
    if (category.isSelected) return
    category.isSelected = true
    categoryStore.update((categorys) => {
      eachCategory(categorys)
      return categorys
    })
  }
  onMount(() => {})
</script>

<div class="categoryItem">
  <div class="line" style={`left:${category.level * 12 + 5}px`} />
  <div on:click|preventDefault|stopPropagation={categoryClick} class={`categoryTitle ${category.isSelected ? 'selected' : ''}`} style={`padding-left:${category.level * 12}px`}>
    <div on:click|preventDefault|stopPropagation={() => (category.isExpanded = !category.isExpanded)} class="expandBtn">
      <i class={`icon ${category.isExpanded ? 'iconremoveRect' : 'iconaddRect'}`} />
    </div>
    <div>{category.title}</div>
  </div>
  {#if category.subCategory && category.isExpanded}
    {#each category.subCategory as subCategory (subCategory.id)}
      <svelte:self category={subCategory} />
    {/each}
  {/if}
</div>

<style lang="scss">
  .categoryItem {
    line-height: 26px;
    position: relative;
  }
  .line {
    content: ' ';
    position: absolute;
    top: 19px;
    bottom: -8px;
    width: 2px;
    border-left: 1px solid #ddd;
    box-sizing: border-box;
  }
  .categoryItem:last-child > .line {
    bottom: 10px !important;
  }
  .categoryTitle {
    display: flex;
    .expandBtn {
      width: 16px;
      cursor: pointer;
      color: #999;
      //
      i {
        font-size: 10px;
        background: rgb(219, 237, 255);
      }
    }
  }
  .categoryTitle:hover {
    background: rgba(121, 184, 255, 0.3);
  }
  .selected {
    background: rgb(121, 184, 255) !important;
    color: #fff;
  }
</style>
