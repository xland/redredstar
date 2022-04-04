<script lang="ts"></script>

<div class="categoryTree">
  <div class="categoryItem">
    <div class="categoryTitle">标题</div>
    <div class="categoryItem">
      <div class="categoryTitle">标题</div>
      <div class="categoryItem">
        <div class="categoryTitle">标题</div>
      </div>
    </div>
    <div class="categoryItem">
      <div class="categoryTitle">标题</div>
    </div>
    <div class="categoryItem">
      <div class="categoryTitle">标题</div>
    </div>
  </div>
  <div class="categoryItem">
    <div class="categoryTitle">标题</div>
    <div class="categoryItem">
      <div class="categoryTitle">标题</div>
      <div class="categoryItem">
        <div class="categoryTitle">标题</div>
      </div>
    </div>
    <div class="categoryItem">
      <div class="categoryTitle">标题</div>
    </div>
    <div class="categoryItem">
      <div class="categoryTitle">标题</div>
    </div>
  </div>
</div>

<style lang="scss">
  .categoryTree {
    background: rgb(219, 237, 255);
    border-right: 1px solid rgb(220, 228, 235);
    flex: 1;
  }
  .categoryItem {
    padding-left: 8px;
    line-height: 26px;
  }
  .categoryTitle:hover {
    background: rgb(33, 136, 255);
    color: #fff;
  }
</style>
