<script lang="ts">
  import { onMount } from 'svelte'

  import { Category } from '../../../model/Category'
  import CategoryTreeNode from './CategoryTreeNode.svelte'
  let categorys: Category[] = []
  let initCategory = () => {
    for (let x = 0; x < 3; x++) {
      let category = new Category()
      category.id = `${x}`
      category.title = `分类${category.id}`
      category.order = x
      category.subCategory = []
      category.level = 1
      category.isExpanded = true
      category.isSelected = x === 1
      categorys.push(category)
      for (let y = 0; y < 6; y++) {
        let category1 = new Category()
        category1.id = `${x}-${y}`
        category1.title = `分类${category1.id}`
        category1.order = y
        category1.parentId = `${x}`
        category1.level = 2
        category1.isExpanded = false
        category1.subCategory = []
        category.subCategory.push(category1)
        for (let z = 0; z < 8; z++) {
          let category2 = new Category()
          category2.id = `${x}-${y}-${z}`
          category2.title = `分类${category2.id}`
          category2.order = z
          category2.parentId = `${x}-${y}`
          category2.level = 3
          category2.isExpanded = false
          category1.subCategory.push(category2)
        }
      }
    }
    categorys = categorys
    console.log('12121')
  }
  onMount(() => {
    initCategory()
  })
</script>

<div class="categoryTree">
  {#each categorys as category (category.id)}
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
