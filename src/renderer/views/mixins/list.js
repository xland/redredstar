import tags from "../../components/tags";
export default {
    components: {
        tags
    },
    data() {
        return {
            searchTags: [],
            searchText: '',
            searchFocus: false,
        }
    },
    methods: {
        closeTag(index) {
            this.searchTags.splice(index, 1);
            this.search();
        },
        removeSearchTag(tagId) {
            let index = this.searchTags.findIndex(v => v.id == tagId);
            if (index < 0) return;
            this.searchTags.splice(index, 1);
            this.search();
        },
        beforeRouteLeave(to, from, next) {
            document.querySelector(".indexListContainer").removeEventListener('scroll', this.handleScroll);
            next();
        },
        handleScroll() {
            var dom = document.querySelector(".indexListContainer");
            if (dom.scrollHeight - dom.scrollTop - dom.offsetHeight < 2) {
                this.search(true);
            }
        },
    },
    mounted() {
        document.querySelector(".indexListContainer").addEventListener('scroll', this.handleScroll);
        this.search();
        this.bus.$on('removeTag', tagId => this.removeSearchTag(tagId));
        this.bus.$on('newArticleAdd', this.search)
    }
}