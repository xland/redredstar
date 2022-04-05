<script lang="ts">
  import { onMount } from 'svelte'
  import { CategoryModel } from '../../../model/CategoryModel'
  import { categoryStore } from '../../Store/categoryStore'
  import CategoryTreeNode from './CategoryTreeNode.svelte'
  onMount(() => {})
</script>

<div class="categoryTree">
  {#each $categoryStore as category (category.id)}
    <CategoryTreeNode {category} />
  {/each}
</div>

<style lang="scss">
  .categoryTree {
    background: rgb(219, 237, 255);
    border-right: 1px solid rgb(220, 228, 235);
    flex: 1;
    overflow: auto;
  }
</style>
