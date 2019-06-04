<style scoped>
.recent {
  position: absolute;
  border-top: 1px solid #e5e5e5;
  line-height: 32px;
  right: 8px;
  top: 45px;
  background: #fff;
  box-shadow: 0 1px 3px rgba(26, 26, 26, 0.2);
  border-bottom-left-radius: 6px;
  border-bottom-right-radius: 6px;
  padding-top: 6px;
  padding-bottom: 6px;
  z-index: 999;
}
.recent .item {
  padding-left: 8px;
  padding-right: 8px;
  max-width: 256px;
  min-width: 120px;
  text-align: center;
  text-overflow: ellipsis;
  white-space: nowrap;
  overflow: hidden;
  color: #666;
  cursor: pointer;
}
.recent .item:hover {
  background: #dcedfe;
  color: #007acc;
}
.noData{
    cursor: default !important;
}
.noData:hover{
    background: #fff !important;
    color: #666 !important;
}
</style>
<template>
  <div v-show="show" @mouseenter="show = true" @mouseleave="show = false">
    <div v-if="recent.length>0" class="recent">
      <div
        :key="item.id"
        @click="$router.push(`/${refer}/${item.id}`)"
        v-for="item in recent"
        class="item"
      >{{item.title}}</div>
    </div>
    <div v-else class="recent">
      <div class="item noData">暂无最近访问的记录</div>
    </div>
  </div>
</template>
<script>
export default {
  props: ["refer", "id"],
  data() {
    return {
      show: false,
      recent: []
    };
  },
  mounted() {
    this.getData();
  },
  methods: {
    async getData() {
      await this.db(this.refer+"s")
        .where("id", this.id)
        .update({
          visited_at: new Date()
        });
      this.recent = await this.db(this.refer+"s")
        .orderBy("visited_at", "desc")
        .limit(8)
        .offset(1);
    }
  }
};
</script>

