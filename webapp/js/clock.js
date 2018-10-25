
function Clock(time) {
    this.time = time
    this.curTime = time
    this.started = false
    this.paused = false
    this.init()
}

Clock.prototype.init = function(){
    this.minCount = Math.floor(this.time / 60 % 60);

    this.secCount = 0;
    this.secCount = Math.floor(this.time % 60);

    if (this.minCount < 10) {
        this.minCount = '0' + this.minCount
    } else {
        this.minCount = this.minCount + ''
    }

    if (this.secCount < 10) {
        this.secCount = '0' + this.secCount
    } else {
        this.secCount = this.secCount + ''
    }
}

Clock.prototype.reset = function(time){
    this.pause()
    if(time){
        this.time = time
    }
    this.curTime = this.time
    this.started = false
    this.paused = false
    this.init()
}

Clock.prototype.start = function () {
    if (this.started) {
        this.continue()
    } else {
        this.timer = setInterval(() => this.countDown(), 1000);
        this.started = true
        console.log(this)
    }
}

Clock.prototype.onEnd = function () { }

Clock.prototype.continue = function () {
    if (this.paused)
        this.timer = setInterval(() => this.countDown(), 1000);
}

Clock.prototype.countDown = function () {
    this.secCount--
    if(this.curTime > 0){
        this.curTime--
    }
    if (this.secCount < 10) {
        this.secCount = '0' + this.secCount
    } else {
        this.secCount = this.secCount + ''
    }

    if (this.secCount.length >= 3) {
        this.secCount = 59;
        if (this.minCount < 10) {
            this.minCount = '0' + (Number(this.minCount) - 1)
        } else {
            this.minCount = (Number(this.minCount) - 1) + ''
        }
    }

    if (this.minCount.length >= 3) {
        this.minCount = '00';
        this.secCount = '00';
        clearInterval(this.timer);
        if (this.onEnd) {
            this.onEnd()
        }
    }
    if (this.perSec) {
        this.perSec(this.toString())
    }
}

Clock.prototype.perSec = function () {

}

Clock.prototype.toString = function () {
    return this.minCount + ':' + this.secCount
}

Clock.prototype.pause = function () {
    if (this.started) {
        clearInterval(this.timer);
        this.paused = true
    }
}
