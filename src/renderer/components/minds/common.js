export default {
    data() {
        return {
            isSelected: false,
            isEdit: false,
            timeStamp: new Date().getTime(),
            editTxt: ''
        }
    },
    mounted() {
        this.cancelSelect();
    },
    methods: {
        nodeClick(event) {
            let ts = new Date().getTime();
            let span = ts - this.timeStamp;
            this.timeStamp = ts;
            if (span < 300) {
                this.editTxt = this.node.data.text;
                this.isEdit = true;
                return;
            }
            this.bus.$emit('cancelSelect', this.node.data.id);
            this.isSelected = !this.isSelected;
        },
        cancelSelect() {
            let self = this;
            this.bus.$on('cancelSelect', id => {
                if (id != self.node.data.id && self.isSelected) {
                    self.isSelected = false;
                }
                if (self.isEdit) {
                    self.isEdit = false;
                    self.node.data.text = self.editTxt;
                    self.$nextTick(() => {
                        let w = document.querySelector(`#${self.node.data.id} > .gRec > text`).getBBox().width + 24;
                        let span = w - self.node.data.w;
                        let nodes = self.node.children || self.node.children_temp;
                        if (self.node.data.x < 0) {
                            self.node.data.x -= span;
                            nodes.forEach(element => {
                                element.data.x -= span;
                            });
                        } else {
                            nodes.forEach(element => {
                                if(element.data.x > 0){
                                    element.data.x += span;
                                }
                            });
                        }
                        self.node.data.w = w;
                    })
                }
            });
        },
        switchPath(flag) {
            var arr = document.querySelectorAll(`#${this.node.data.id} path`);
            for (let dom of arr) {
                dom.style.display = flag;
            }
        },
        getNodeHeight(node) {
            if (node.children_temp) return 30;
            return document.getElementById(node.data.id).getBBox().height;
        },
        reLocation() {
            this.switchPath('none');
            let index = 0;
            let cur = this.node.children[index];
            let preHeight = this.getNodeHeight(cur);
            let y = 0
            cur.data.y = y;
            cur = this.node.children[index += 1];
            while (cur) {
                let curHeight = this.getNodeHeight(cur);
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
            this.$nextTick(() => {
                this.$parent.reLocation(this.node.data.x > 0);
            })
        },
        addSubNode(x) {
            if (this.node.children_temp) {
                this.node.children = this.node.children_temp;
                delete this.node.children_temp;
            }
            let newNode = {
                data: {
                    "id": this.node.data.id + "_" + Math.floor(Math.random() * 1000000),
                    "created": new Date().getTime(),
                    "text": "",
                    "y": 0,
                    "w": 72,
                    "h": 30,
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