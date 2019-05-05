export default {
    data() {
        return {
            isSelected: false
        }
    },
    mounted() {
        this.cancelSelect();
    },
    methods: {
        nodeSelect() {
            this.bus.$emit('cancelSelect', this.node.data.id);
            this.isSelected = !this.isSelected;
        },
        cancelSelect() {
            let self = this;
            this.bus.$on('cancelSelect', id => {
                if (id != self.node.data.id && self.isSelected) {
                    self.isSelected = false;
                }
            });
        },
        switchPath(flag) {
            var arr = document.getElementsByClassName(this.node.data.id);
            for (let dom of arr) {
                dom.style.display = flag;
            }
        },
        reLocation() {
            this.switchPath('none');
            let index = 0;
            let cur = this.node.children[index];
            let preHeight = document.getElementById(cur.data.id).getBBox().height;
            let y = 0
            cur.data.y = y;
            cur = this.node.children[index += 1];
            while (cur) {
                let curHeight = document.getElementById(cur.data.id).getBBox().height;
                y += curHeight / 2 + 60 + preHeight / 2;
                cur.data.y = y;
                preHeight = curHeight;
                cur = this.node.children[index += 1];
            }
            let center = y / 2;
            index = 0;
            cur = this.node.children[index];
            while (cur) {
                cur.data.y -= center;
                cur = this.node.children[index += 1];
            }
            this.switchPath('inherit');
            if (this.$parent.reLocation)
                this.$parent.reLocation(this.node.data.x > 0);
        },
        addSubNode(x) {
            let newNode = {
                data: {
                    "id": this.node.data.id + "_" + Math.floor(Math.random() * 1000000),
                    "created": new Date().getTime(),
                    "text": "",
                    "y": 0,
                    x
                },
                children: []
            }
            this.node.children.push(newNode);
            if (this.node.children.length > 1)
                this.$nextTick(() => this.reLocation(x > 0));
        }
    }
}