// Copyright (c) 2022 Zhendong Peng (pzd17@tsinghua.org.cn)
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "gflags/gflags.h"
#include "glog/logging.h"
#include "processor/processor.h"

#include "frontend/g2p_prosody.h"
#include "frontend/wav.h"
#include "model/tts_model.h"
#include "utils/string.h"

DEFINE_string(tagger, "", "tagger fst file");
DEFINE_string(verbalizer, "", "verbalizer fst file");

DEFINE_string(vocab, "", "tokenizer vocab file");

DEFINE_string(char2pinyin, "", "chinese character to pinyin");
DEFINE_string(pinyin2id, "", "pinyin to id");
DEFINE_string(pinyin2phones, "", "pinyin to phones");
DEFINE_string(g2p_prosody_model, "", "g2p prosody model file");

DEFINE_string(speaker2id, "", "speaker to id");
DEFINE_string(phone2id, "", "phone to id");
DEFINE_string(sname, "", "speaker name");
DEFINE_string(vits_model, "", "e2e tts model file");

DEFINE_string(text, "", "input text");
DEFINE_string(wav_path, "", "output wave path");

int main(int argc, char* argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, false);
  google::InitGoogleLogging(argv[0]);

  auto tn = std::make_shared<wetext::Processor>(FLAGS_tagger, FLAGS_verbalizer);
  auto g2p_prosody = std::make_shared<wetts::G2pProsody>(
      FLAGS_g2p_prosody_model, FLAGS_vocab, FLAGS_char2pinyin, FLAGS_pinyin2id,
      FLAGS_pinyin2phones);
  auto model = std::make_shared<wetts::TtsModel>(
      FLAGS_vits_model, FLAGS_speaker2id, FLAGS_phone2id, tn, g2p_prosody);

  std::vector<float> audio;
  int sid = model->GetSid(FLAGS_sname);
  model->Synthesis(FLAGS_text, sid, &audio);

  wetts::WavWriter wav_writer(audio.data(), audio.size(), 1, 22050, 16);
  wav_writer.Write(FLAGS_wav_path);
  return 0;
}
